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
*   Modifed Time: 2019年06月08日 星期六 15时53分54秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/
#include "RBTree.h"
#include <stdexcept>
#include <cassert>

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

// 红黑树的插入操作
bool RBTree::insert(int nValue_)
{
	// 如果红黑树为空时，直接插入即可。
	if (!m_pRoot)
	{
		m_pRoot = new RBNode();
		m_pRoot->m_nValue = nValue_;
		m_pRoot->m_nColor = BLACK;
		++m_nSize;
		return true;
	}

	// 首先查找插入位置
	RBNode* _pParent = nullptr;		// 插入位置的父结点
	RBNode** _ppNode = &m_pRoot;	// 指向插入位置的指针。
	while (*_ppNode)
	{
		// 更新父结点
		_pParent = *_ppNode;

		// 如果已经存在，则返回false,不需要插入。
		if (nValue_ == (*_ppNode)->m_nValue)
			return false;
		else if (nValue_ < (*_ppNode)->m_nValue)
			_ppNode = &(*_ppNode)->m_pLeft;
		else
			_ppNode = &(*_ppNode)->m_pRight;
	}

	// 进行插入操作
	*_ppNode = new RBNode();
	(*_ppNode)->m_nValue = nValue_;
	(*_ppNode)->m_pParent = _pParent;
	(*_ppNode)->m_nColor = RED;

	// 调整红黑树，使其满足红黑树的性质, 并递增元素数目.
	insert_fixup(*_ppNode);
	++m_nSize;
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

void RBTree::transplant(RBNode* pOldNode_, RBNode* pNewNode_)
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

// 左旋操作
void RBTree::left_rotate(RBNode* pNode_)
{
	// 左旋时，当前结点以及右子结点一定不能为空。
	if (!pNode_ || !pNode_->m_pRight)
		return;

	RBNode* _pRight = pNode_->m_pRight;
	// 使用右子结点的左子树代替右子结点的位置
	transplant(_pRight, _pRight->m_pLeft);
	// 使用右子结点代替当前结点
	transplant(_pRight, pNode_);
	// 把当前结点作为右子结点的左子树的位置
	pNode_->m_pParent = _pRight;
	_pRight->m_pLeft = pNode_;
}

// 右旋操作
void RBTree::right_rotate(RBNode* pNode_)
{
	// 右旋时，当前结点以及左子结点一定不能为空。
	if (!pNode_ || !pNode_->m_pLeft)
		return;

	RBNode* _pLeft = pNode_->m_pLeft;
	// 使用左子结点的右子树代替左子结点的位置
	transplant(_pLeft, _pLeft->m_pRight);
	// 使用左子结点代替当前结点的位置
	transplant(pNode_, _pLeft);
	// 使用当前结点代替左子结点右子树的位置。
	pNode_->m_pParent = _pLeft;
	_pLeft->m_pRight = pNode_;
}

// 插入之后调整红黑树，使其符合红黑树的性质
// 其中，pNode_是新插入的结点。
void RBTree::insert_fixup(RBNode* pNode_)
{
	if (!pNode_)
		return;

	// 只有当父结点存在，并且父结点的颜色为红色时才会破坏红黑树的性质，这时需
	// 要进行调整。
	while (true)
	{
		RBNode* _pParent = pNode_->m_pParent;
		// 如果父结点不存在，则说明该结点为根结点，置为黑色，返回即可。
		if (!_pParent)
		{
			pNode_->m_nColor = BLACK;
			return;
		}

		// 如果父结点的颜色为黑色，不需要调整，直接返回。
		if (_pParent->m_nColor == BLACK)
			return;

		// 父结点为红色时，祖结点一定存在，并且祖结点的颜色一定是黑色的。
		RBNode* _pGrandpa = _pParent->m_pParent;
		assert(_pGrandpa);
		assert(_pGrandpa->m_nColor == BLACK);
		// 如果叔父结点为红色时，改变父结点与叔父结点为黑色，祖结点为红色，并
		// 沿着树向上继续判断祖结点。
		if (sibling(_pParent)->m_nColor == RED)
		{
			_pParent->m_nColor = BLACK;
			sibling(_pParent)->m_nColor = BLACK;
			_pGrandpa->m_nColor = RED;
			pNode_ = _pGrandpa;
			continue;
		}
		else	// 叔父结点为黑色时，需要旋转和改变颜色。
		{
			// 父结点改为黑色，祖结点改为红色
			_pParent->m_nColor = BLACK;
			_pGrandpa->m_nColor = RED;
			// 如果父结点为祖结点的左子结点，则对祖结点进行右旋
			if (_pGrandpa->m_pLeft == _pParent)
			{
				// 如果当前结点为父结点的右结点时，需要先对父结点进行左旋一下
				// 因为如果仅仅对祖结点进行右旋不满足红黑树的条件。
				if (_pParent->m_pRight == pNode_)
					left_rotate(_pParent);
				right_rotate(_pGrandpa);
			}
			else
			{
				if (_pParent->m_pLeft == pNode_)
					right_rotate(_pParent);
				left_rotate(_pGrandpa);
			}

			// 此时直接停止，不需要向上检测了。
			break;
		}
	}
}

// 删除之后调整红黑树，使其符合红黑树的性质
void RBTree::erase_fixup(RBNode* pNode_)
{

}

// 获取兄弟结点,不存在时，返回空。
RBNode* RBTree::sibling(RBNode* pNode_) const
{
	// 给定结点为空为父结点不存在时，返回nullptr.
	if (!pNode_ || !pNode_->m_pParent)
		return nullptr;

	if (pNode_ == pNode_->m_pParent->m_pLeft)
		return pNode_->m_pParent->m_pRight;
	else
		return pNode_->m_pParent->m_pLeft;
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
