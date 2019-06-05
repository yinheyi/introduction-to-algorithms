/***********************************************************************
*   Copyright (C) 2019  Yinheyi. <chinayinheyi@163.com>
*   
* This program is free software; you can redistribute it and/or modify it under the terms
* of the GNU General Public License as published by the Free Software Foundation; either 
* version 2 of the License, or (at your option) any later version.

*   Brief:    
*   Author: yinheyi
*   Email: chinayinheyi@163.com
*   Version: 1.0
*   Created Time: 2019年05月18日 星期六 16时38分08秒
*   Modifed Time: 2019年06月05日 星期三 23时37分02秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/
#include <iostream>
#include <cassert>
#define abs(x) ((x) > 0 ? (x) : -1*(x))
#define max(x,y) ((x) > (y) ? (x) : (y))

// 1.平衡二叉搜索树在二叉搜索树的基础上增加了一个限制条件：要求左右子树的高度差不得大于1.
// 2.平衡二叉搜索树又称作AVL树,AVL是它的两个发明者Adelson-Velskii和Landis名字中的字母。
// 3.平衡二叉搜索树的难点在于当二叉树不满足限制条件时，如何调整树的结构, 这里就涉及到结点
// 的左旋和右旋问题。
// 4.对于平衡二叉搜索树的结点来说，需要在二叉搜索树的基础上增加一个表示左右子树高度差的值，
// 使用平衡因子(balance factor, BF)表示，它表示左子树的高度减右子树的高度。节点的定义如下：
// 5. 在二叉搜索树的基础上，对于插入和删除操作，增加了平衡判断和平衡调整。
//
// 补充说明：
// 以左旋为例，稍微说明一下.  具体写代码时并不会分这么多情况的.
// 在二叉搜索树时，发生左旋的情况有2种，一种是当前结点的平衡因子为-2时需要左旋。另一种当
// 需要双旋操作时，第一次旋转可能需要左旋，此时它的平衡因子为-1.
// 情况一：当前结点的平衡因子为-2, 右子结点的平衡因子为0时：
//          1                                   3
//            \           对1左旋              /  \
//             3      =============>          1    5
//            /  \                             \
//           4    5                             4
//   此时情况只会发生在删除节点时,  a. 树的高度不变。
//                                  b. 当前结点的平衡因子由-2变为-1.
//                                  c. 右子结点的平衡因子由0变为1.
//
// 情况二：当前结点的平衡因子为-2, 右子结点的平衡因子为-1时：
//           1                             
//            \        对1左旋            
//             2     ============>          2
//              \                          /  \
//               3                        1    3
//   此种情况可能发生在删除和插入时，a. 树的高度减少1
//                                   b. 当前结点的平衡因子由-2变为0 
//                                   c. 右子结点的平衡因子由-1变为0
//
// 情况三：当前结点的平衡因子为-2, 右子结点的平衡因子为-2时：
//          1                                1                             4
//        /   \                            /   \                         /   \
//       2     3          对3右旋         2     4         对1左旋       1     3
//           /   \      ==========>               \     ========>      /     /  \
//          4     5                                3                  2     6    5
//           \                                   /  \
//            6                                 6     5
//   此种情况发生在双旋的第二次是左旋的时, a. 树的高度减少1
//                                         b. 当前结点的平衡因子由-2变为1
//                                         c. 右子结点的平衡因子由-2变为0
//
// 情况四：当前结点的平衡因子为-1, 右子结点的平衡因子为0时：
//            1                                 1
//           /                                 /
//          2           对2进行左旋           3
//           \       ==============>         /
//            3                             2
//    此种情况发生在双旋的第一次为左旋时，a. 树的高度不变
//                                        b. 当前结点的平衡因子由-1变为0
//                                        c. 右子结点的平衡因子由0变为1
//
// 情况五：当前结点的平衡因子为-1, 右子结点的平衡因子为-1时：
//        1                                      1
//       /  \                                   /  \
//      2    3          对2进行左旋            5    3
//     / \            ==========>            /  \
//    4   5                                 2    6
//         \                              /
//          6                            4
//   此种情况下发生在第一次为左旋的时候， a. 树的高度不变
//                                        b. 当前结点的平衡因子由-1变为1
//                                        c. 左子结点的平衡因子由-1变为1.
//
// 情况六： 当前结点的的平衡因子为-1, 右子结点的平衡因子为1时：
//          1                                      1
//        /  \                                    /  \
//       2    3          对2进行左旋             5    3
//     /  \            ==============>          /
//    4    5                                   2
//        /                                   / \
//       6                                   4   6
//  此种情况下发生在双旋的第一次为左旋时， a. 树的高度不变
//                                         b. 当前结点的平衡因子由-1变为0
//                                         c. 右子结点的平衡因子由1变为2.
//
//
/************* 平衡二叉搜索树的节点定义  *************/
struct Node
{
	int m_nValue;
	int m_nHight;   // 此处表示以该结点表示的树的高度,如果换成平衡因子的话也可以，但是在
                    // 旋转过程中需要去维护平衡因子的变化，很麻烦。
	Node* m_pParent;
	Node* m_pLeft;
	Node* m_pRight;

	// 构造函数
	Node(int nValue_ = 0, int nHigh_ = 1, 
		Node* pParent_ = nullptr, Node* pLeft_ = nullptr, Node* pRight_ = nullptr);
	// 获取该结点的平衡因子
	int BF();
	// 更新当前结点的高度
	void UpdateHight();
};

// node的构造函数。
Node::Node(int nValue_, int nHigh_, Node* pParent_, Node* pLeft_, Node* pRight_)
{
	m_nValue = nValue_;
	m_nHight = nHigh_; 
	m_pParent = pParent_;
	m_pLeft = pLeft_;
	m_pRight = pRight_;
}

// 获取结点的平衡因子
int Node::BF()
{
	int _nLeftHight = m_pLeft ? m_pLeft->m_nHight : 0;
	int _nRightHight = m_pRight ? m_pRight->m_nHight : 0;
	return _nLeftHight - _nRightHight;
}

// 更新当前结点的高度
void Node::UpdateHight()
{
	int _nLeftHight = m_pLeft ? m_pLeft->m_nHight : 0;
	int _nRightHight = m_pRight ? m_pRight->m_nHight : 0;
	m_nHight = 1 + max(_nLeftHight, _nRightHight);
}

/********     平衡二叉搜索树类的定义    **********/
class AVLTree
{
public:
	AVLTree();
	~AVLTree();
	size_t size();
	Node* search(int nValue_);
	void insert(int nValue_);
	void erase(Node* pNode_);

private:
	Node* precursor(Node* pNode_);		// 返回前驱节点, 不存在返回空
	Node* successor(Node* pNode_);		// 获取后驱节点，不存在返回空
	void left_rotate(Node* pNode_);		// 对给定结点进行左旋
	void right_rotate(Node* pNode_);	// 对给定结点进行右旋
	void delete_tree(Node* pRoot_);
	void Transplant(Node* pOldNode_, Node* pNewNode);	// 使用pNewNode_表示的树代替pOldNode_表示的树

	Node* m_pRoot;
	size_t m_nSize;
};

// AVL的构造函数
AVLTree::AVLTree()
{
	m_pRoot = nullptr;
	m_nSize = 0;
}

// 析构函数
AVLTree::~AVLTree()
{
	delete_tree(m_pRoot);
}

// 获取平衡二叉搜索树中的节点个数
size_t AVLTree::size()
{
	return m_nSize;
}

// 给定一个值，返回该值所在的节点指针，如果不存在，则返回空
// 基于循环实现, 当然基于递归实现代码更简单一些，但是效率稍差一点。
Node* AVLTree::search(int nValue_)
{
	// 如果树为空，则返回nullptr.
	if (!m_pRoot)
	{
		return nullptr;
	}

	// 如果与当前结点的值相同，则返回当前结点指针
	// 如果小于当前结点的值，则去左子树进行继续查找
	// 如果大于当前结点的值，则去右子树进行继续查找
	Node* _pCurrent = m_pRoot;
	while (_pCurrent)
	{
		if (nValue_ == _pCurrent->m_nValue)
			return _pCurrent;
		else if (nValue_ < _pCurrent->m_nValue)
			_pCurrent = _pCurrent->m_pLeft;
		else
			_pCurrent = _pCurrent->m_pRight;
	}

	// 如果没有在循环内返回的话，说明不存在，返回nullptr.
	return nullptr;
}

// 插入操作： 如果待拷入的值已经存在于AVL树中，则什么也不做。
void AVLTree::insert(int nValue_)
{
	// 1. 当树为空时，直接插入作为根结点即可
	if (!m_pRoot)
	{
		m_pRoot = new Node(nValue_, 1);		// 0表示该结点的高度。
		return;
	}

	// 2. 先查找插入位置的父结点，该过程类似于查找操作
	Node* _pCurrent = m_pRoot;
	while (true)
	{
		// _pCurrent结点不可能为空，不需要判断。
		if (nValue_ < _pCurrent->m_nValue && _pCurrent->m_pLeft)
		{
			 _pCurrent = _pCurrent->m_pLeft;
			 continue;
		}

		if (nValue_ > _pCurrent->m_nValue && _pCurrent->m_pRight)
		{
			_pCurrent = _pCurrent->m_pRight;
			continue;
		}

		if (nValue_ == _pCurrent->m_nValue)
			return;
	}

	// 3. 进行插入操作, 并更新父结点的平衡因子
	if (nValue_ < _pCurrent->m_nValue)
	{
		_pCurrent->m_pLeft = new Node(nValue_, 1);

		// 如果右子树为空，则需要增加当前结点高度
		if (!_pCurrent->m_pRight)
			++_pCurrent->m_nHight;
	}
	else
	{
		_pCurrent->m_pRight = new Node(nValue_, 1);

		// 如果左子树为空，则需要增加当前结点的高度
		if (!_pCurrent->m_pLeft)
			++_pCurrent->m_nHight;
	}

	// 接下来，是插入的重点: 当插入后，可能增加了子树的高度，从而破坏了AVL树的平衡性质，
	// 我们需要作出调整, 分以下情况哈：
	// 1. 当插入后，父结点的平衡因子为0,说明没有增加父结点表示的子树的高度，同时也没有
	// 破坏父结点的平衡性，不需要调整。
	// 2. 当插入后，父结点的平衡因子为1或-1,此时，虽然没有破坏父结点的平衡性，但是增加
	// 了父结点表示子树的高度，此时需要沿着树向上去判断祖结点.....
	// 3. 当插入后，父结点的平衡因子不会出现为-2或2的情况。
	// 4. 第2步中的祖结点会可能会出现-2或2的情况，此时需要通过左旋/右旋/双旋进行调整。
	

	// 平衡因子不为0时，插入操作增加了当前结点的高度，可能会引起不平衡，所以需要进行
	// 相关判断与操作
	while (_pCurrent->BF() != 0)
	{
		// 此时，插入操作增加了当前结点的高度，需要更新父结点的高度，并继续检测父结点
		if (_pCurrent->BF() == 1 || _pCurrent->BF() == -1)
		{
			Node* _pParent = _pCurrent->m_pParent;
			// 当父结点为空时，不需要调整，直接返回了。
			if (!_pParent)
				return;

			// 更新父结点的高度
			_pParent->UpdateHight();

			// 继续去检测父结点的平衡因子.
			_pCurrent = _pCurrent->m_pParent;
			continue;
		}

		else if (_pCurrent->BF() == 2)
		{
			// 此时需要根据左子结点的平衡因子判断是进行单旋还是双旋
			// 因为是插入操作，此时的左子结点的平衡因子不会出现0的情况.
			// 当左子结点为平衡因为为1时，只需要对当前结点作一个右旋即可。
			// 当左子结点的平衡因子为-1时，需要先对左子结点进行左旋操作，再对当前结点
			// 进行右旋操作。
			if (_pCurrent->m_pLeft->BF() == -1)
				left_rotate(_pCurrent->m_pLeft);
			right_rotate(_pCurrent);

			// 旋转操作之后，会使当前结点表示的子树高度与没有被破坏平衡之前的高度
			// 相同，因此没有必要继续向上检查，直接退出即可。
			return;
		}
		else if (_pCurrent->BF() == -2)
		{
			if (_pCurrent->m_pRight->BF() == 1)
				right_rotate(_pCurrent->m_pRight);
			left_rotate(_pCurrent);
			return;
		}
	}

	++m_nSize;
}

// AVL树的删除操作:删除给定结点的指针。
// 对于删除的话，是有点麻烦。真的让人头皮发麻。总的方针是先删除，再调整。
void AVLTree::erase(Node* pNode_)
{
	// 参数不合法时，直接返回
	if (!pNode_)
		return;

	
	// 该指针变量用于保存在删除过程中高度最低的那个被影响的结点, 该结点的高
	// 度会发生变化. 在后续调整树的平衡度时，从该结点开始调整起。该结点被初
	// 始化为被删除结点的父结点。
	Node* _pImplicatedNode = pNode_->m_pParent;

	// 删除时，分几种情况：
	// 1. 如果结点没有左右子树时，直接删除就可以。
	// 2. 如果只有左子树时，使用左子代替即可。
	// 3. 如果只存在右子树时，使用右子树代替即可。
	// 4. 如果都存在时，使用后驱结点来替换（使用前驱结点来替换也没有问题）
	if (!pNode_->m_pLeft && !pNode_->m_pRight)    // 左右子树都为空
		Transplant(pNode_, nullptr);
	else if (!pNode_->m_pLeft)    // 左子树为空
		Transplant(pNode_, pNode_->m_pRight);
	else if (!pNode_->m_pRight)		// 右子树为空
		Transplant(pNode_, pNode_->m_pLeft);
	else 
	{
		Node* _pSuccessorNode = successor(pNode_);	// 后驱结点
		assert(_pSuccessorNode);
		// 如果后驱结点正好是删除结点的右子结点时，需要处理一下
		if (pNode_->m_pRight == _pSuccessorNode)
		{
			assert(!_pSuccessorNode->m_pLeft);
			_pSuccessorNode->m_pLeft = pNode_->m_pLeft;
			pNode_->m_pLeft->m_pParent = _pSuccessorNode;
			Transplant(pNode_, _pSuccessorNode);
			_pImplicatedNode = _pSuccessorNode;		// 该结点的高度发生了变化
		}
		else
		{
			assert(!_pSuccessorNode->m_pLeft);
			// 处理后驱结点的右子树
			Transplant(_pSuccessorNode, _pSuccessorNode->m_pRight);
			_pImplicatedNode = _pSuccessorNode->m_pParent;	// 后驱结点的父结点受影响

			// 使用后驱结点替换将要被删除的结点。
			_pSuccessorNode->m_pLeft = pNode_->m_pLeft;
			_pSuccessorNode->m_pRight = pNode_->m_pRight;
			pNode_->m_pLeft->m_pParent = _pSuccessorNode;
			pNode_->m_pRight->m_pParent = _pSuccessorNode;
			_pSuccessorNode->m_nHight = pNode_->m_nHight;	// 高度也替换一下
			Transplant(pNode_, _pSuccessorNode);
		}
	}

	// 此时的搜索树可能不满足了平衡条件，需要进行性的调整
	while (_pImplicatedNode)
	{
		_pImplicatedNode->UpdateHight();

		// 如果平衡因子为-1或1，说明该结点的高度没有变化, 因此它不会
		// 影响更上面的结点，所以不需要调整的。
		if (abs(_pImplicatedNode->BF() == 1))
			break;

		// 如果平衡因子为0,则说明该结点的高度减少了1,它的父结点要受
		// 影响了，因此需要继续检测父结点。
		if (_pImplicatedNode->BF() == 0)
		{
			_pImplicatedNode = _pImplicatedNode->m_pParent;
			continue;
		}

		// 如果平衡因子为2或-2.则需要右旋或双旋操作
		// 在进行旋转之前，我们先记住一下当前结点为父结点的左子结点
		// 还是右子结点，这样的话，我们才能找到旋转之后的结点。
		Node* _pParent = _pImplicatedNode->m_pParent;
		bool _bIsLeft = true;		// 先暂时初始化为true, 
		if (_pParent && _pParent->m_pRight == _pImplicatedNode)
			_bIsLeft = false;
		int _nOldHight = _pImplicatedNode->m_nHight;		// 旋转之前的高度

		// 保存好了旋转之前的状态以后，接下来进行旋转操作了
		if (_pImplicatedNode->BF() == 2)
		{
			// 如果左子结点的平衡因子为-1,则需要先左旋左子结点
			if (_pImplicatedNode->m_pLeft->BF() == -1)	// 不可能等于-2的
				left_rotate(_pImplicatedNode->m_pLeft);
			right_rotate(_pImplicatedNode);
		}
		else
		{
			if (_pImplicatedNode->m_pRight->BF() == 1)
				right_rotate(_pImplicatedNode->m_pRight);
			left_rotate(_pImplicatedNode);
		}

		// 判断是否需要继续沿树向上调整
		// a. 之前保存的父结点为空时，不需要调整了，已经到顶了。
		if (!_pParent)
			break;
		// b.如果旋转前后树的高度没有变化，则不会影响其它结点，退出即可。
		int _nNewHight = _bIsLeft ? _pParent->m_pLeft->m_nHight : _pParent->m_pRight->m_nHight;
		if (_nOldHight == _nNewHight)
			break;

		// 那只好再继续向上检测调整了。
		_pImplicatedNode = _pParent;
	}

	--m_nSize;
}

// 获取前驱结点，不存在时，返回空。
Node* AVLTree::precursor(Node* pNode_)
{
	// 如果指针无效，则返回空。
	if (!pNode_)
		return nullptr;

	// 如果该结点有左子树的话，则前驱结点一定是左子树中值最大的结点
	if (pNode_->m_pLeft)
	{
		// 沿着左子树的右侧一直找下去即可。
		pNode_ = pNode_->m_pLeft;
		while (pNode_->m_pRight)
			pNode_ = pNode_->m_pRight;

		return pNode_;
	}

	// 如果该结点没有左子树的话，则需要沿着树向上查找，直到某个结点
	// 为其父结点的右结点为止，则父结点就是前驱结点.
	while (pNode_->m_pParent)
	{
		if (pNode_->m_pParent->m_pRight == pNode_)
			return pNode_->m_pParent;
		else
			pNode_ = pNode_->m_pParent;
	}

	// 如果程序走到此处，说明没有查找到，返回nullptr.
	return nullptr;
}

// 返回后驱结点，不存在时，返回空。
Node* AVLTree::successor(Node* pNode_)
{
	if (!pNode_)
		return nullptr;

	// 如果有右子树，则后驱结点对应了右子树中值最小的结点
	if (pNode_->m_pRight)
	{
		pNode_ = pNode_->m_pRight;
		while (pNode_->m_pLeft)
			pNode_ = pNode_->m_pLeft;

		return pNode_;
	}

	// 不存在右子树时，沿着树向上查找，直到某一个结点为其父结点的
	// 左子结点为止。父结点即为后驱结点.
	while (pNode_->m_pParent)
	{
		if (pNode_->m_pParent->m_pLeft == pNode_)
			return pNode_->m_pParent;
		else
			pNode_ = pNode_->m_pParent;
	}

	// 程序直到此处时，说明没有查找到，返回nullptr.
	return nullptr;
}

// 左旋操作
void AVLTree::left_rotate(Node* pNode_)
{
	if (!pNode_ || !pNode_->m_pRight)
		return;

	Node* _pParent = pNode_->m_pParent;	 	// 父结点
	Node* _pRightNode = pNode_->m_pRight;	// 右子结点

	// 使用右子结点的左子树代替当前的右子结点。
	pNode_->m_pRight = _pRightNode->m_pLeft;
	if (pNode_->m_pRight)
		pNode_->m_pRight->m_pParent = pNode_;
	pNode_->UpdateHight();

	// 把当前结点作为右子结点的左子树
	_pRightNode->m_pLeft = pNode_;
	pNode_->m_pParent = _pRightNode;
	_pRightNode->UpdateHight();

	// 更新右子结点与父结点的关系
	_pRightNode->m_pParent = _pParent;
	if (_pParent  && _pParent->m_pLeft == pNode_)	// 父结点不为空且为父结点的左子结点
	{
		_pParent->m_pLeft = _pRightNode;
		_pParent->UpdateHight();
	}
	else if (_pParent && _pParent->m_pRight == pNode_)	//父结点不为空且为父结点的右子结点
	{
		_pParent->m_pRight = _pRightNode;
		_pParent->UpdateHight();
	}
	else   // 如果父结点为空，则需要更新根结点
		m_pRoot = _pRightNode;
}

// 右旋操作
void AVLTree::right_rotate(Node* pNode_)
{
	if (!pNode_ || !pNode_->m_pLeft)
		return;

	Node* _pParent = pNode_->m_pParent;		// 父结点
	Node* _pLeftNode = pNode_->m_pLeft;		// 左子结点

	// 使用左子结点的右子树代替左子结点
	pNode_->m_pLeft = _pLeftNode->m_pRight;
	if (pNode_->m_pLeft)
		pNode_->m_pLeft->m_pParent = pNode_;
	pNode_->UpdateHight();

	// 使用当前结点代替左子结点的右子树
	_pLeftNode->m_pRight = pNode_;
	pNode_->m_pParent = _pLeftNode;
	pNode_->UpdateHight();

	// 更新左子结点与父结点的关系
	_pLeftNode->m_pParent = _pParent;
	if (_pParent && _pParent->m_pLeft == pNode_)	// 父结点不为空且为父结点的左子结点 
	{
		_pParent->m_pLeft = _pLeftNode;
		_pParent->UpdateHight();
	}
	else if (_pParent && _pParent->m_pRight == pNode_)	//父结点不为空且为父结点的右子结点
	{
		_pParent->m_pRight = _pLeftNode;
		_pParent->UpdateHight();
	}
	else	// 父结点为空时，更新根结点
		m_pRoot = _pLeftNode;
}

// 删除该AVL树,释放内存
void AVLTree::delete_tree(Node* pRoot_)
{
	// 指针为空时，返回
	if (!pRoot_)
		return;

	// 当左右子树不为空时，先递归调用左右子树
	if (pRoot_->m_pLeft)
		delete_tree(pRoot_->m_pLeft);
	if (pRoot_->m_pRight)
		delete_tree(pRoot_->m_pRight);

	delete pRoot_;
	pRoot_ = nullptr;
}

// 使用pNewNode_表示的树代替pOldNode_表示的树
// 其中pNewNode_可以为空的, 所以下面的代码时刻要注意pNewNode_为空的时候
void AVLTree::Transplant(Node* pOldNode_, Node* pNewNode_)
{
	if (!pOldNode_)
		return;

	// 如果旧结点为根结点，则需要修改根结点的值。
	if (pOldNode_ == m_pRoot)
	{
		assert(!pOldNode_->m_pParent);
		m_pRoot = pNewNode_;
		if (pNewNode_)
		pNewNode_->m_pParent = nullptr;

		return;
	}


	// 关联旧结点的父结点与新结点之间的关系
	if (pOldNode_->m_pParent->m_pLeft == pOldNode_)
		pOldNode_->m_pParent->m_pLeft = pNewNode_;
	else
		pOldNode_->m_pParent->m_pRight = pNewNode_;
	if (pNewNode_)
		pNewNode_->m_pParent = pOldNode_->m_pParent;
}

/**********************    测试程序     *************************/
int main(int argc, char* argv[])
{
	// 代码是否有问题，还没有进行测试。。
	// 如何测试？？
	return 0;
}
