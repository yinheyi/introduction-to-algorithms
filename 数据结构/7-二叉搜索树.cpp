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
*   Created Time: 2019年05月18日 星期六 16时37分28秒
*   Modifed Time: 2019年05月19日 星期日 22时56分39秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/
#include <iostream>
#include <cassert>		// 使用assert()断言

// 1. 二叉搜索树是在广义的二叉树地基础上增加了限制条件得到的,它要求树中的结点满足：
// 它的左子树的上的值都小于等于该结点的值，它的右子树的值都大于等于该结点的值。
// 增加了该限制条件之后，我们很可以很方便地在O(lonN)时间内(N表示总结点数）查找一
// 个给定的值. 因此把它叫作二叉搜索树. 例如下图为一个二叉搜索树：
//                                 12
//                              /      \
//                            4         19
//                          /   \      /   \
//                        -2     9    15    100
//                             /        \
//                            7          16
// 2. 由于二叉搜索树的特点，我们使用二叉树的中序遍历正好可以把二叉搜索树中的元素
// 按从小到大的顺序进行遍历一遍。
// 3. 二叉搜索树常使用的一些操作包含：
//     a. 查找给定的值，如果存在则返回结点的指针，如果不存在则返回为nullptr
//     b. 获取最小值
//     c. 获取最大值
//     d. 求给定结点的前驱结点
//     e. 求给定结点的后驱结点
//     f. 插入元素
//     g. 删除元素
// 4. 由于在进行求前驱结点/后驱结点/删除元素等操作时，需要通过子结点获取父结点的指针，
// 因此，在结点的struct内增加指向父结点的指针一项，即node的定义如下：
struct Node
{
	int m_nValue;
	Node* m_pParent;
	Node* m_pLeft;
	Node* m_pRight;
	Node(int value_ = 0, Node* pParent_ = nullptr, Node* pLeft_ = nullptr, Node* pRight_ = nullptr);
};

// node的构造函数
Node::Node(int value_, Node* pParent_, Node* pLeft_, Node* pRight_)
{
	m_nValue = value_;
	m_pParent = pParent_;
	m_pLeft = pLeft_;
	m_pRight = pRight_;
}
 
// 二叉搜索树的查找函数: 输入根结点指针和要查找的值, 如果值存在则返回那个结点的指针，
// 如果不存在则返回nullptr.
Node* Search(Node* pRoot_, int nValue_)
{
	Node* _pCurrentNode = pRoot_;
	while (_pCurrentNode != nullptr)
	{
		// 如果相等，则退出循环，返回当前结点的指针
		if (nValue_ == _pCurrentNode->m_nValue)
			break;
		// 如果小于当前结点的值，接下来去左子树中去查找
		else if (nValue_ < _pCurrentNode->m_nValue)
			_pCurrentNode = _pCurrentNode->m_pLeft;
		// 如果大于当前结点的值，接下来去右子树中去查找
		else
			_pCurrentNode = _pCurrentNode->m_pRight;
	}

	return _pCurrentNode;
}

// 二叉搜索树查找最小值所在结点的操作, 如果二叉树为空，则返回nullptr.
// 查找最小值时，一直沿着左结点一路查到到头即可。
Node* Minimum(Node* pRoot_)
{
	if (pRoot_ == nullptr)
		return nullptr;

	Node* _pMin = pRoot_;
	while (_pMin->m_pLeft != nullptr)
		_pMin = _pMin->m_pLeft;

	return _pMin;
}

// 二叉搜索树查找最大值所在结点的操作, 如果二叉树为空，则返回nullptr.
// 查找最大值时，一直沿着右结点一路查到到头即可。
Node* Maximum(Node* pRoot_)
{
	if (pRoot_ == nullptr)
		return nullptr;

	Node* _pMax = pRoot_;
	while (_pMax->m_pRight != nullptr)
		_pMax = _pMax->m_pRight;

	return _pMax;
}

// 求二叉搜索树给定结点的前驱结点：
// 前驱结点就是在中序遍历的顺序中给定结点的前一个结点。
// 1. 当给定结点的左子树不为空时，前驱结点为左子树中最大值所在的结点。
// 2. 当给定结点的左子树为空时，有点麻烦,又要分两种情况：
//    a. 如果当前结点的父结点的右结点，则前驱结点就是父结点；
//    b. 如果当前结点为父结点的左子结点，则需要沿着二叉树一直向上找，直到找
//       到一个祖先为右结点为止, 返回当祖先结点的父结点。如果没有找到，则说
//       明当前结点就是最小值所有的结点，因此不存在前驱结点。
//    在实际编程时,可以把过程a与b两种情况合二为一来处理。
Node* PrecursorNode(Node* pNode_)
{
	if (nullptr == pNode_)
		return nullptr;

	if (pNode_->m_pLeft != nullptr)
		return Maximum(pNode_->m_pLeft);
	else
	{
		// 当父结点不为空并且当前结点不是父结点的右子结点时，则一直沼着二叉树向上查找
		while (pNode_->m_pParent != nullptr && pNode_->m_pParent->m_pRight != pNode_)
		{
			pNode_ = pNode_->m_pParent;
		}

		return pNode_->m_pParent;
	}
}

// 求二叉搜索树给定结点的后驱结点与查找前驱结点的代码是对称的：
// 后驱结点就是在中序遍历的顺序中给定结点的后一个结点。
// 1. 当给定结点的右子树不为空时，后驱结点为右子树中最小值所在的结点。
// 2. 当给定结点的右子树为空时，有点麻烦,又要分两种情况：
//    a. 如果当前结点的父结点的左结点，则后驱结点就是父结点；
//    b. 如果当前结点为父结点的右子结点，则需要沿着二叉树一直向上找，直到找
//       到一个祖先为左结点为止, 返回当祖先结点的父结点。如果没有找到，则说
//       明当前结点就是最大值所有的结点，因此不存在后驱结点。
//    在实际编程时,可以把过程a与b两种情况合二为一来处理。
Node* SuccessorNode(Node* pNode_)
{
	if (nullptr == pNode_)
		return pNode_;

	if (pNode_->m_pRight != nullptr)
		return Minimum(pNode_->m_pRight);
	else
	{
		// 如果当前结点的父结点不为空并且当前结点为父结点的左子结点时，一下沿二叉树向上查找
		while (pNode_->m_pParent != nullptr && pNode_->m_pParent->m_pLeft != pNode_)
		{
			pNode_ = pNode_->m_pParent;
		}

		return pNode_->m_pParent;
	}
}

// 二叉搜索树的插入操作:
// 1. 插入操作会修改二叉搜索树. 例如：如果给定的根结点为空，插入之后需要修改根结点的值。因此，传
// 参的时候，应该使用指向指针的指针或指向指针的引用。
// 2. 插入操作时，一定是把新new出来的结点插入到最后一层结点的叶结点上, 就可以满足二叉搜索树的性质，
// 所以不存在插入到中间的情况。
// 3. 插入操作的核心是找到待插入的位置：从根结点开始，遇到键值大的则向左，遇到键值小的则向右，一直
// 到尾端，即为插入点。
void Insert(Node*& pRoot_, int nValue_)
{
	// 先new出来一个新的结点
	Node* _pNewNode = new Node(nValue_);

	// 如果根结点为空时，把新结点的指针赋值给根结点即可。
	if (pRoot_ == nullptr)
	{
		pRoot_ = _pNewNode;
		return;
	}

	// 查找到查入位置的父结点
	Node* _pParent = pRoot_;
	while (_pParent->m_pLeft != nullptr || _pParent->m_pRight != nullptr)
	{
		if (nValue_ < _pParent->m_nValue && _pParent->m_pLeft != nullptr)
		{
			_pParent = _pParent->m_pLeft;
			continue;
		}
		else if(nValue_ >= _pParent->m_nValue && _pParent->m_pRight != nullptr)
		{
			_pParent= _pParent->m_pRight;
			continue;
		}
		else
			break;
	}

	// 进行插入操作
	if (nValue_ < _pParent->m_nValue)
	{
		assert(nullptr == _pParent->m_pLeft);
		_pParent->m_pLeft = _pNewNode;
		_pNewNode->m_pParent = _pParent;
	}
	else
	{
		assert(nullptr == _pParent->m_pRight);
		_pParent->m_pRight = _pNewNode;
		_pNewNode->m_pParent = _pParent;
	}
}

// 二叉搜索树的删除操作
// 1. 让我们想想什么时候可以直接删除一个结点而不需要考虑其它结点呢？那就是当该结点的
// 左右子结点都为空时。此时直接删除该结点是OK的。如果该结点存在左子树和右子树时，我们
// 需要调整其它结点, 具体来说：
//     a. 当左右子结点都不存在时，直接删除即可。
//     b.当只存在左子树时，使用左子树代替被删除的结点。
//     c.当只存在右子树时，使用右子树代替被删除的结点。
//     d.当左右子树都存在时，使用左子树的最大值结点或右子树的最小值结点来代替被删除的结点。
// 2. 为了方便处理移动一个结点时它的左右子树的处理，定义一个子树的移植函数, 函数的原型为：
// void Transplant(Node*& pRoot_, Node* pOld_, Node* pNew_);
//    a. 因为该函数可能会修改到根结点的指针，所以使用指针的引用来传递根结点的指针。
//    b. 旧结点(除了是根结点以外）不能为空，因为如果为空的话，我们不知道它的父结点是谁。
//       而新结点可以为空，即表示使用一个空结点来代替旧结点。
//    c. 子树移动时，不仅仅是移动一个结点，与该结点绑定的左右子树都会跟着移动。
//
void Transplant(Node*& pRoot_, Node* pOldNode_, Node* pNewNode_)
{
	if (pRoot_ == pOldNode_)
	{
		pRoot_ = pNewNode_;
		return;
	}

	if (nullptr == pRoot_ || nullptr == pOldNode_)
		return;

	// 通过上面两个if条件的过虑之后，旧结点的父结点不可能为空，即旧结点不可能为根结点。
	assert(pOldNode_->m_pParent != nullptr);
	if (pOldNode_ == pOldNode_->m_pParent->m_pLeft)
	{
		pOldNode_->m_pParent->m_pLeft = pNewNode_;
	}
	else
	{
		pOldNode_->m_pParent->m_pRight = pNewNode_;
	}

	if (pNewNode_ != nullptr)
		pNewNode_->m_pParent = pOldNode_->m_pParent;
}

// 二叉搜索树的删除操作
void Delete(Node*& pRoot_, Node* pDeleteNode_)
{
	if (nullptr == pRoot_ || nullptr == pDeleteNode_)
		return;

	if (pDeleteNode_->m_pLeft == nullptr || pDeleteNode_->m_pRight == nullptr)
	{
		// 使用空结点代替被删除的结点。
		Transplant(pRoot_, pDeleteNode_, nullptr);
	}
	else if (pDeleteNode_->m_pRight == nullptr)
	{
		Transplant(pRoot_, pDeleteNode_, pDeleteNode_->m_pLeft);
	}
	else if (pDeleteNode_->m_pLeft == nullptr)
	{
		Transplant(pRoot_, pDeleteNode_, pDeleteNode_->m_pRight);
	}
	else
	{
		// 从左子树中查找代替结点, 使用代替结点的左子树来替换代替结点所表示的树.
		Node* _pAlternateNode = Maximum(pDeleteNode_->m_pLeft);
		assert(_pAlternateNode != nullptr);
		assert(_pAlternateNode->m_pRight == nullptr);
		Transplant(pRoot_, _pAlternateNode, _pAlternateNode->m_pLeft);

		// 修改代替结点的左右子结点的值，并使用代替结点表示的新树来替换准备删除的结点
		// 表示的树。 即使代替结点为准备删除结点的左子结点，下面的代码也是OK的。因为在
		// 这种情况下，上面的transplant()函数已经修改了pDeleteNode的左子结点的值.
		// 左子结点
		_pAlternateNode->m_pLeft = pDeleteNode_->m_pLeft;
		// 当代替结点是准备删除结点的左子结点时, 代替结点的新左子结点可能为空，所以
		// 在使用该指针时一定要进行非空判断。
		if (_pAlternateNode->m_pLeft != nullptr)
			_pAlternateNode->m_pLeft->m_pParent = _pAlternateNode;

		// 右子结点
		_pAlternateNode->m_pRight = pDeleteNode_->m_pRight;
		assert(_pAlternateNode->m_pRight != nullptr);
		_pAlternateNode->m_pRight->m_pParent = _pAlternateNode;

		Transplant(pRoot_, pDeleteNode_, _pAlternateNode);
	}

	delete pDeleteNode_;
	pDeleteNode_ = nullptr;
}

/**********************    测试程序     *************************/
typedef void (*pFunc)(Node*);
void BuildBinaryTree(Node*& pRoot_, std::istream& cin_);
void PrintNode(Node* pNode_);
void InorderTraversal_ByRecursion(Node* pRoot_, pFunc CallbackFunc_);

int main(int argc, char* argv[])
{
	std::cout << "输入二叉搜索树的前序遍历结果, 建立一个二叉搜索树. 假设所有节点的为正整数 " << std::endl;
	std::cout << "如果结点的元素为空, 则使用0表示。" << std::endl;
	std::cout << "例如： 4 1 0 2 0 0 5 0 0 为下图的二叉搜索树的前序遍历结果：" << std::endl;
	std::cout << "               4" << std::endl;
	std::cout << "            /     \\" << std::endl;
	std::cout << "           1       5" << std::endl;
	std::cout << "             \\" << std::endl;
	std::cout << "              2" << std::endl;

	std::cout << "你的输入序列为： ";
	Node* _pRoot = nullptr;
	BuildBinaryTree(_pRoot, std::cin);

	std::cout << "你新建的二叉搜索树的元素从小到大排序为：";
	InorderTraversal_ByRecursion(_pRoot, PrintNode);
	std::cout << std::endl;

	// 测试求最大值与最小值的正确性：
	std::cout << "当前二叉搜索树中最小值为："; 
	PrintNode(Minimum(_pRoot)); 
	std::cout << std::endl;
	std::cout << "当前二叉搜索树中最大值为："; 
	PrintNode(Maximum(_pRoot)); 
	std::cout << std::endl;

	// 测试插入代码：
	std::cout << "请输入一个正整数，插入到二叉搜索树中：";
	int _nTemp = 0;
	while (!(std::cin >> _nTemp) || _nTemp <= 0)
	{
		std::cin.clear();
		std::cout << "你傻B 啊，没有看到要求吗？？让你输入正整数，你输入的是什么玩意！重新输入：";
	}
	Insert(_pRoot, _nTemp);
	std::cout << "插入之后的二叉树的元素从小到大打印为：";
	InorderTraversal_ByRecursion(_pRoot, PrintNode);
	std::cout << std::endl;

	//测试查找与删除操作：
	std::cout <<"请输入你要删除的正整数：";
	while (!(std::cin >> _nTemp) || _nTemp <= 0)
	{
		std::cin.clear();
		std::cout << "你怎么还是傻B 啊，又没有看到要求吗？？让你输入正整数，你输入的是什么玩意！重新输入：";
	}
	Node* _pTemp = Search(_pRoot, _nTemp);
	if (nullptr == _pTemp)
	{
		std::cout << "你要想删除的数在二叉搜索树中不存在！！！" << std::endl;
	}
	else
	{
		Delete(_pRoot, _pTemp);
		std::cout << "删除之后，二叉搜索树中元素从小到大打印为：";
		InorderTraversal_ByRecursion(_pRoot, PrintNode);
		std::cout << std::endl;

	}

	return 0;
}


// 输入二叉搜索树的前序遍历结果, 建立一个二叉搜索树. 假设所有节点的为正整数, 
// 如果结点的元素为空, 则使用0表示。
// 例如： 4 1 0 2 0 0 5 0 0 为下图的二叉搜索树的前序遍历结果：
//                4
//             /     \\
//            1       5
//              \\
//               2
//
//
void BuildBinaryTree(Node*& pRoot_, std::istream& cin_)
{
	int _nTemp;
	if (cin_ >> _nTemp)
	{
		if (_nTemp > 0)
		{
			pRoot_ = new Node(_nTemp);
			BuildBinaryTree(pRoot_->m_pLeft, cin_);
			if (pRoot_->m_pLeft != nullptr)
				pRoot_->m_pLeft->m_pParent = pRoot_;

			BuildBinaryTree(pRoot_->m_pRight, cin_);
			if (pRoot_->m_pRight!= nullptr)
				pRoot_->m_pRight->m_pParent = pRoot_;
		}
	}
}

// 该函数实现打印给定二叉树中结点的值。
void PrintNode(Node* pNode_)
{
	if (pNode_ == nullptr)
		return;
	std::cout << pNode_->m_nValue << " ";
}

void InorderTraversal_ByRecursion(Node* pRoot_, pFunc CallbackFunc_)
{
	if (nullptr == pRoot_ || nullptr == CallbackFunc_)
		return;

	// 先递归遍历左子树
	InorderTraversal_ByRecursion(pRoot_->m_pLeft, CallbackFunc_);
	// 其次遍历父结点
	CallbackFunc_(pRoot_);
	// 最后递归遍历右子树
	InorderTraversal_ByRecursion(pRoot_->m_pRight, CallbackFunc_);
}
