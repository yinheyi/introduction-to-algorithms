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
*   Created Time: 2019年05月18日 星期六 16时38分00秒
*   Modifed Time: 2019年06月08日 星期六 11时26分15秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/
#include "RBTree.h"
#include <stdexcept>

// 红黑树结点的构造函数
RBNode::RBNode()
{
	m_nValue = 0;
	m_nColor = RED;
	m_pParent = nullptr;
	m_pLeft = nullptr;
	m_pRight = nullptr;
}

// 构造函数
RBTree::RBTree()
{
	m_nSize = 0;
	m_pRoot = nullptr;
}

// 析构函数
RBTree::~RBTree()
{
	free_memory(m_pRoot);
	m_pRoot = nullptr;
}

// 查找给定值的结点，如果找不到，返回空
RBNode* RBTree::search(int nValue_) const
{
	RBNode* _pNode = m_pRoot;
	while (_pNode)
	{
		if (nValue_ < _pNode->m_nValue)
			_pNode = _pNode->m_pLeft;
		else if (nValue_ > _pNode->m_nValue)
			_pNode = _pNode->m_pRight;
		else
			break;
	}

	return _pNode;
}

bool RBTree::insert(int nValue_)
{

}

bool RBTree::erase(RBNode* pNode_)
{

}

size_t RBTree::size() const
{
	return m_nSize;
}

bool RBTree::empty() const
{
	return m_nSize == 0;
}

int RBTree::min() const
{
	RBNode* _pMinNode = min_node(m_pRoot);
	if (!_pMinNode)
		throw std::out_of_range("红黑树不空，不存在最小值。");

	return _pMinNode->m_nValue;
}

int RBTree::max() const
{
	RBNode* _pMaxNode = max_node(m_pRoot);
	if (!_pMaxNode)
		throw std::out_of_range("红黑树为空，不存在最大值。");

	return _pMaxNode->m_nValue;
}

void RBTree::Transplant(RBNode* pOldNode_, RBNode* pNewNode_)
{
	// 如果原结点等于根结点，则直接把新的node赋值为根结点
	if (pOldNode_ == m_pRoot)
		m_pRoot = pNewNode_;

	// 此时，如果旧结点为空，直接返回。
	if (!pOldNode_)
		return;

	// 正式开始榢接
	RBNode* _pParent = pOldNode_->m_pParent;
	pNewNode_->m_pParent = _pParent;
	if (_pParent)
	{
		if(_pParent->m_pLeft == pOldNode_)
			_pParent->m_pLeft = pNewNode_;
		else
			_pParent->m_pRight = pNewNode_;
	}
}

// 求前驱结点
RBNode* RBTree::precursor(RBNode* pNode_) const
{
	if (!pNode_)
		return nullptr;

	// 如果左子树不为空，前驱结点一定是左子树中最大值的结点。
	if (pNode_->m_pLeft)
		return max_node(pNode_->m_pLeft);

	// 此时，就需要沿着二叉树向上查找，直到某一个结点为其父结点的右
	// 结点，则其父结点就是前驱结点。
	while (pNode_->m_pParent)
	{
		if (pNode_->m_pParent->m_pRight == pNode_)
			break;
		pNode_ = pNode_->m_pParent;
	}

	return pNode_->m_pParent;
}

// 求后驱结点
RBNode* RBTree::successor(RBNode* pNode_) const
{
	if (!pNode_)
		return nullptr;

	// 如果右子树不为空，后驱结点一定是右子树中最小值的结点。
	if (pNode_->m_pRight)
		return min_node(pNode_->m_pRight);

	// 沿着二叉树向上查找，直到某一个结点是父结点的左结点为止。则其父
	// 结点就是后驱结点。
	while (pNode_->m_pParent)
	{
		if (pNode_->m_pParent->m_pLeft == pNode_)
			break;
		pNode_ = pNode_->m_pParent;
	}

	return pNode_->m_pParent;
}

RBNode* RBTree::min_node(RBNode* pNode_) const
{
	if (!pNode_)
		return nullptr;

	while (pNode_->m_pLeft)
		pNode_ = pNode_->m_pLeft;

	return pNode_;
}

RBNode* RBTree::max_node(RBNode* pNode_) const
{
	if (!pNode_)
		return nullptr;

	while (pNode_->m_pRight)
		pNode_ = pNode_->m_pRight;

	return pNode_;
}

// 采用递归来解决
void RBTree::free_memory(RBNode* pNode_)
{
	if (pNode_ == nullptr)
		return;

	free_memory(pNode_->m_pLeft);
	pNode_->m_pLeft = nullptr;
	free_memory(pNode_->m_pRight);
	pNode_->m_pRight = nullptr;
	delete pNode_;
}
