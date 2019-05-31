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
*   Modifed Time: 2019年05月31日 星期五 23时54分55秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/
#include <iostream>
#include <cassert>
#define abs(x) ((x) > 0 ? (x) : -1*(x))

// 1.平衡二叉搜索树在二叉搜索树的基础上增加了一个限制条件：要求左右子树的高度差不得大于1.
// 2.平衡二叉搜索树又称作AVL树,AVL是它的两个发明者Adelson-Velskii和Landis名字中的字母。
// 3.平衡二叉搜索树的难点在于当二叉树不满足限制条件时，如何调整树的结构, 这里就涉及到结点
// 的左旋和右旋问题。
// 4.对于平衡二叉搜索树的结点来说，需要在二叉搜索树的基础上增加一个表示左右子树高度差的值，
// 使用平衡因子(balance factor, BF)表示，它表示左子树的高度减右子树的高度。节点的定义如下：
// 5. 在二叉搜索树的基础上，对于插入和删除操作，增加了平衡判断和平衡调整。
//
//
//
/************* 平衡二叉搜索树的节点定义  *************/
struct Node
{
	int m_nValue;
	int m_nBF;		// 对于平衡二叉树来说，平衡因子为-1/0/1. (左子树高度-右子树的高度)
	Node* m_pParent;
	Node* m_pLeft;
	Node* m_pRight;
	Node(int nValue_ = 0, int nBF_ = 0, 
		Node* pParent_ = nullptr, Node* pLeft_ = nullptr, Node* pRight_ = nullptr);
};

// node的构造函数。
Node::Node(int nValue_, int nBF_, Node* pParent_, Node* pLeft_, Node* pRight_)
{
	m_nValue = nValue_;
	m_nBF = nBF_;
	m_pParent = pParent_;
	m_pLeft = pLeft_;
	m_pRight = pRight_;
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
	Node* precursor();					// 返回前驱节点, 不存在返回空
	Node* successor();					// 获取后驱节点，不存在返回空
	void left_rotate(Node* pNode_);		// 对给定结点进行左旋
	void right_rotate(Node* pNode_);	// 对给定结点进行右旋
	void delete_tree(Node* pRoot_);

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
	// 如果大于当前结点的值，则去右子树进行疆场查找
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
	// 当树为空时，直接插入作为根结点即可
	if (!m_pRoot)
	{
		m_pRoot = new Node(nValue_, 0);		// 0表示平衡因子
		return;
	}

	// 先查找插入位置的父结点，该过程类似于查找操作
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

	// 进行插入操作, 并更新父结点的平衡因子
	if (nValue_ < _pCurrent->m_nValue)
	{
		_pCurrent->m_pLeft = new Node(nValue_, 0);
		++_pCurrent->m_nBF;
	}
	else
	{
		_pCurrent->m_pRight = new Node(nValue_, 0);
		--_pCurrent->m_nBF;
	}

	// 接下来，是插入的重点: 当插入后，可能增加了子树的高度，从而破坏了AVL树的平衡性质，
	// 我们需要作出调整, 分以下情况哈：
	// 1. 当插入后，父结点的平衡因子为0,说明没有增加父结点表示的子树的高度，同时也没有
	// 破坏父结点的平衡性，不需要调整。
	// 2. 当插入后，父结点的平衡因子为1或-1,此时，虽然没有破坏父结点的平衡性，但是增加
	// 了父结点表示子树的高度，此时需要沿着树向上去判断祖结点.....
	// 3. 当插入后，父结点的平衡因子不会出现为-2或2的情况。
	// 4. 第2步中的祖结点会可能会出现-2或2的情况，此时需要通过左旋/右旋/双旋进行调整。
	//
	// 平衡因子为0时，直接返回，不需要其它任何操作。
	while (_pCurrent->m_nBF != 0)
	{
		if (_pCurrent->m_nBF == 1 || _pCurrent->m_nBF == -1)
		{
			// 当祖结点为空时，不需要调整，直接返回了。
			if (!_pCurrent->m_pParent)
				return;

			// 根据父结点是祖结点的左孩子还是右孩子，去调整祖结点的平衡因子
			if (_pCurrent == _pCurrent->m_pParent->m_pLeft)
				++_pCurrent->m_pParent->m_nBF;
			else
				--_pCurrent->m_pParent->m_nBF;

			// 继续去检测祖结点的平衡因子.
			_pCurrent = _pCurrent->m_pParent;
		}
		else if (_pCurrent->m_nBF == 2)
		{
			// 此时需要根据左子结点的平衡因子判断是进行单旋还是双旋
			// 因为是插入操作，此时的左子结点的平衡因子不会出现0的情况.
			// 当左子结点为平衡因为为1时，只需要对当前结点作一个右旋即可。
			// 当左子结点的平衡因子为-1时，需要先对左子结点进行左旋操作，再对当前结点
			// 进行右旋操作。
			if (_pCurrent->m_pLeft->m_nBF == -1)
				left_rotate(_pCurrent->m_pLeft);
			right_rotate(_pCurrent);

			// 旋转操作之后，会使当前结点表示的子树高度与没有被破坏平衡之前的高度
			// 相同，因此没有必要继续向上检查，直接退出即可。
			return;
		}
		else if (_pCurrent->m_nBF == -2)
		{
			if (_pCurrent->m_pRight->m_nBF == 1)
				right_rotate(_pCurrent->m_pRight);
			left_rotate(_pCurrent);
			return;
		}
	}
}

// AVL树的删除操作:删除给定结点的指针。
void AVLTree::erase(Node* pNode_)
{

}

// 获取前驱结点，不存在时，返回空。
Node* AVLTree::precursor()
{
}

// 返回后驱结点，不存在时，返回空。
Node* AVLTree::successor()
{
}

// 关于左旋与右旋操作
// 在二叉搜索树时，发生左旋的情况有2种，一种是当前结点的平衡因子为-2时需要左旋。另一种当
// 需要双旋操作时，第一次旋转也需要左旋，此时它的平衡因子为-1.
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
//   此种情况可能发生在删除和插入时，a. 树的高度不变
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
//          2           对1进行左旋           3
//           \       ==============>         /
//            3                             2
//    此种情况发生在双旋的第一次为左旋时，a. 树的高度不变
//                                        b. 当前结点的平衡因子由-1变为0
//                                        c. 右子结点的平衡因子由0变为1
//
// 情况五：
//
//
// 
//
//
//
//
//
// 左旋操作
void AVLTree::left_rotate(Node* pNode_)
{

}

// 右旋操作
void AVLTree::right_rotate(Node* pNode_)
{

}

// 删除该AVL树,释放内存
void delete_tree(Node* pRoot_)
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

/**********************    测试程序     *************************/
int main(int argc, char* argv[])
{
	return 0;
}

