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
*   Created Time: 2019年05月13日 星期一 23时22分10秒
*   Modifed Time: 2019年05月18日 星期六 23时26分10秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/
#include <stack>
#include <queue>
#include <iostream>
#include <cassert>

// 1. 二叉树，顾名思义，有两个树杈， 也就是一个结点有两个子结点,可能某个子结点为空。下图就是
// 一个二叉树：
//                         1
//                      /      \
//                     2         4
//                   /   \         \
//                 5      6         7
//                                /   \
//                               22    1
//
// 2. 最广义的二叉树通常是用于学习与理解这种数据结构的, 在实际应用中都是使用附加了限制条件的
// 二叉树，比如：平衡二叉搜索树/红黑树等。
// 3. 如果二叉树的每一层节点个数都是满的，则称为满二叉树;如果二叉树除了最底层之外其它层都是满
// 的， 并且最底层也是从左到右填充的，则为完全二叉树。
// 4. 对于二叉树，通常会涉及到：
//    a. 二叉树的前序遍历:先访问父结点，再访问左结点，最后访问右结点。
//    b. 二叉树的中序遍历:先访问左结点，再访问父结点，最后访问右结点。
//    c. 二叉树的后序遍历:先访问左结点，再访问右结点，最后访问父结点。
//    补充说明:上面说的前中后是针对访问父结点的顺序来说的。
//    d. 二叉树的层次遍历，也就是一层层的遍历
// 5. 单向链表可以看作是一种特殊的二叉树.
// 6. 对于那些关于树的高度/给定n个结点求树的最大高度最小高度等相关的知识没有啥意思，自己推导
// 一下就知道啦。
// 7. 接下来二叉树的四种遍历算法,在大多数数据结构与算法书中，给出的前序/中序/后序遍历的代码
// 都是使用递归来实现的, 使用递归来实现的代码非常简洁明了。 这里，我们同时给出基于循环的前序/
// 中序/后序的遍历方法, 基于循环的遍历方法会直接使用到栈数据结构。对于层次遍历算法，是基于队
// 列的数据结构来实现。


/* 二叉树结点的定义: 为了简单，不使用模板，直接使用int类型 */
struct BinaryNode
{
	// 构造函数
	BinaryNode(int value_ = 0, BinaryNode* pLeft_ = nullptr, BinaryNode* pRight_ = nullptr);
	int m_value;
	BinaryNode* m_pLeft;
	BinaryNode* m_pRight;
};

BinaryNode::BinaryNode(int value_, BinaryNode* pLeft_, BinaryNode*pRight_)
{
	m_value = value_;
	m_pLeft = pLeft_;
	m_pRight = pRight_;
}

// 定义一个回调函数指针类型
typedef void (*pFunc)(BinaryNode*);

/***************     基于递归的前序遍历      *******************/
void PreorderTraversal_ByRecursion(BinaryNode* pRoot_, pFunc CallbackFunc_)
{
	if (nullptr == pRoot_ || nullptr == CallbackFunc_)
		return;

	// 遍历父结点
	CallbackFunc_(pRoot_);
	// 递归遍历左子树
	PreorderTraversal_ByRecursion(pRoot_->m_pLeft, CallbackFunc_);
	// 递归遍历右子树
	PreorderTraversal_ByRecursion(pRoot_->m_pLeft, CallbackFunc_);
}

/***************     基于循环的前序遍历      *******************/
void PreorderTraversal_ByLoop(BinaryNode* pRoot_, pFunc CallbackFunc_)
{
	if (nullptr == pRoot_ || nullptr == CallbackFunc_)
		return;

	// 定义一个栈, 并把根结点的指针存放在栈中
	std::stack<BinaryNode*> _stackContainter;
	_stackContainter.push(pRoot_);

	// 进行遍历操作
	while (!_stackContainter.empty())
	{
		// 遍历栈顶元素并出栈
		BinaryNode* _pCurrentNode = _stackContainter.top();
		assert(_pCurrentNode != nullptr);
		CallbackFunc_(_pCurrentNode);
		_stackContainter.pop();

		// 如果右子树与左子树不为空，则入栈。注意：一定要先添加右子树，
		// 再添加左子树, 这样才能保证出栈的时候先遍历左子树再遍历右子树.
		if (_pCurrentNode->m_pRight != nullptr)
			_stackContainter.push(_pCurrentNode->m_pRight);
		if (_pCurrentNode->m_pLeft != nullptr)
			_stackContainter.push(_pCurrentNode->m_pLeft);
	}
}

/***************     基于递归的中序遍历      *******************/
void InorderTraversal_ByRecursion(BinaryNode* pRoot_, pFunc CallbackFunc_)
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

/***************     基于循环的中序遍历      *******************/
void InorderTraversal_ByLoop(BinaryNode* pRoot_, pFunc CallbackFunc_)
{
	if (nullptr == pRoot_ || nullptr == CallbackFunc_)
		return;

	std::stack<BinaryNode*> _stackContainer;

	// 一路找下去，直到找到最左的子结点
	while (pRoot_ != nullptr)
	{
		_stackContainer.push(pRoot_);
		pRoot_ = pRoot_->m_pLeft;
	}

	while (!_stackContainer.empty())
	{
		// 遍历栈顶的结点
		BinaryNode* _pCurrentNode = _stackContainer.top();
		CallbackFunc_(_pCurrentNode);
		_stackContainer.pop();

		// 接下来，找到当前结点的右子树中最左的子结点
		_pCurrentNode = _pCurrentNode->m_pRight;
		while (_pCurrentNode != nullptr)
		{
			_stackContainer.push(_pCurrentNode);
			_pCurrentNode = _pCurrentNode->m_pLeft;
		}
	}
}

/***************     基于递归的后序遍历      *******************/
void PostorderTraversal_ByRecursion(BinaryNode* pRoot_, pFunc CallbackFunc_)
{
	if (nullptr == pRoot_ || nullptr == CallbackFunc_)
		return;

	// 先递归遍历左子树
	InorderTraversal_ByRecursion(pRoot_->m_pLeft, CallbackFunc_);
	// 其次递归遍历右子树
	InorderTraversal_ByRecursion(pRoot_->m_pRight, CallbackFunc_);
	// 最后遍历父结点
	CallbackFunc_(pRoot_);
}

/***************     基于循环的后序遍历      *******************/
void PostorderTraversal_ByLoop(BinaryNode* pRoot_, pFunc CallbackFunc_)
{
	if (nullptr == pRoot_ || nullptr == CallbackFunc_)
		return;

	std::stack<BinaryNode*> _stackContainer;

	// 一路找下去，一直找到最左的子结点
	while (pRoot_ != nullptr)
	{
		_stackContainer.push(pRoot_);
		pRoot_ = pRoot_->m_pLeft;
	}

	while (_stackContainer.empty())
	{
		// 接下来，不能遍历栈顶的结点，因为如果该结点存在右子树,则需要去遍历右子树上的结点.
		// 只要右子树不为空，就需要找右子树上最左的子结点, 重复该过程直到某个最左子结点的右
		// 子树为空为止。
		while (_stackContainer.top() ->m_pRight != nullptr)
		{
			// 还是一路找右子树上的最左子结点
			BinaryNode* _pCurrentNode = _stackContainer.top()->m_pRight;
			while (_pCurrentNode != nullptr)
			{
				_stackContainer.push(_pCurrentNode);
				_pCurrentNode = _pCurrentNode->m_pLeft;
			}
		}

		// 到此时，我们就可以遍历栈顶的元素了。
		BinaryNode* _pCurrentNode = _stackContainer.top();
		CallbackFunc_(_pCurrentNode);
		_stackContainer.pop();

		// 把当前结点出栈之后，此时栈顶的元素肯定是当前结点的父结点,我们需要判断当前结点是
		// 父结点的左子结点还是右子结点:
		// a. 如果当前结点是父结点的左子结点，接下来我们还需要去遍历父结点的右子结点,即重复
		// 上面的过程。
		// b. 如果当前结点是父结点的右子结点，则接下来我们直接去遍历父结点即可,遍历完父结点后，
		// 还需要继续判断父结点是否为祖结点的右子结点, 重复此过程，直接不满足条件为止。
		while (_pCurrentNode == _stackContainer.top()->m_pRight)
		{
			_pCurrentNode = _stackContainer.top();
			CallbackFunc_(_pCurrentNode);
			_stackContainer.pop();
		}
	}
}

/***************     基于队列的层次遍历      *******************/
void HierarchialTraversal(BinaryNode* pRoot_, pFunc CallbackFunc)
{
	if (nullptr == pRoot_ || nullptr == CallbackFunc)
		return;

	std::queue<BinaryNode*> _queue;
	_queue.push(pRoot_);
	while (!_queue.empty())
	{
		// 遍历队首的元素
		BinaryNode* _pCurrent = _queue.front();
		CallbackFunc(_pCurrent);
		_queue.pop();

		// 添加左右子结点至队尾
		if (nullptr != _pCurrent->m_pLeft)
			_queue.push(_pCurrent->m_pLeft);
		if (nullptr != _pCurrent->m_pRight)
			_queue.push(_pCurrent->m_pRight);
	}
}

/**********************    测试程序     *************************/
void BuildBinaryTree(BinaryNode*& pRoot_, std::istream& cin_);
void PrintNode(BinaryNode* pNode_);

int main(int argc, char* argv[])
{
	BinaryNode* _pRoot = nullptr;
	BuildBinaryTree(_pRoot, std::cin);

	std::cout << "基于递归的前序遍历结果如下：" << std::endl;
	PreorderTraversal_ByRecursion(_pRoot, PrintNode);
	std::cout << std::endl;

	std::cout << "基于循环的前序遍历结果如下：" << std::endl;
	PreorderTraversal_ByLoop(_pRoot, PrintNode);
	std::cout << std::endl;

	std::cout << "基于递归的中序遍历结果如下：" << std::endl;
	InorderTraversal_ByRecursion(_pRoot, PrintNode);
	std::cout << std::endl;

	std::cout << "基于循环的中序遍历结果如下：" << std::endl;
	InorderTraversal_ByLoop(_pRoot, PrintNode);
	std::cout << std::endl;

	std::cout << "基于递归的后序遍历结果如下：" << std::endl;
	PostorderTraversal_ByRecursion(_pRoot, PrintNode);
	std::cout << std::endl;

	std::cout << "基于循环的后序遍历结果如下：" << std::endl;
	PostorderTraversal_ByLoop(_pRoot, PrintNode);
	std::cout << std::endl;

	std::cout << "二叉树的层次遍历结果如下：" << std::endl;
	HierarchialTraversal(_pRoot, PrintNode);
	std::cout << std::endl;
	return 0;
}

// 输入二叉树的前序遍历结果, 建立一个二叉树. 假设所有节点的为正整数, 
// 如果结点的元素为空, 则使用0表示。
// 例如：
void BuildBinaryTree(BinaryNode*& pRoot_, std::istream& cin_)
{
	int _nTemp;
	if (cin_ >> _nTemp)
	{
		if (_nTemp > 0)
		{
			pRoot_ = new BinaryNode(_nTemp);
			BuildBinaryTree(pRoot_->m_pLeft, cin_);
			BuildBinaryTree(pRoot_->m_pRight, cin_);
		}
	}
}

// 该函数实现打印给定二叉树中结点的值。
void PrintNode(BinaryNode* pNode_)
{
	if (pNode_ == nullptr)
		return;
	std::cout << pNode_->m_value << " ";
}
