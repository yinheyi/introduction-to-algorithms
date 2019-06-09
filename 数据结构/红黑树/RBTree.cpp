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
*   Modifed Time: 2019年06月09日 星期日 23时12分31秒
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
	// 参数不合法
	if (!pNode_)
		return false;

	// 如果该结点的左右子结点都为空，用空结点代替。
	if (!pNode_->m_pLeft && !pNode_->m_pRight)
	{
		transplant(pNode_, nullptr);
		if (pNode_->m_nColor == BLACK)
			erase_fixup(nullptr, pNode_->m_pParent);

		delete pNode_;
		return true;
	}

	// 如果右结点为空时，使用左子树代替。
	if (!pNode_->m_pRight)
	{
		transplant(pNode_, pNode_->m_pLeft);
		if (pNode_->m_nColor == BLACK)
		{
			// 如果替代结点为黑色，需要调整。
			// 如果替代结点为红色，则直接设置为黑色就可。
			if (pNode_->m_pLeft->m_nColor == BLACK)
				erase_fixup(pNode_->m_pLeft, pNode_->m_pParent);
			else
				pNode_->m_pLeft->m_nColor = BLACK;
		}

		delete pNode_;
		return true;
	}

	// 如果左结点为空时，使用右子树代替。
	if (!pNode_->m_pLeft)
	{
		transplant(pNode_, pNode_->m_pRight);

		if (pNode_->m_nColor == BLACK)
		{
			if (pNode_->m_pRight->m_nColor == BLACK) 
				erase_fixup(pNode_->m_pRight, pNode_->m_pParent);
			else
				pNode_->m_pRight->m_nColor = BLACK;
		}

		delete pNode_;
		return true;
	}

	// 左右子树都不为空的情况：
	RBNode* _pReplaceNode = min_node(pNode_->m_pRight);
	assert(!_pReplaceNode->m_pLeft);		 // 替换结点的左子结点一定是空的。
	// 如果替换结点为删除结点的右孩子时，
	if (_pReplaceNode == pNode_->m_pRight)
	{
		// 被替换结点原来的颜色
		int _nColor = _pReplaceNode->m_nColor;

		_pReplaceNode->m_pLeft = pNode_->m_pLeft;
		pNode_->m_pLeft->m_pParent = _pReplaceNode;
		_pReplaceNode->m_nColor = pNode_->m_nColor;
		transplant(_pReplaceNode, pNode_);

		if (_nColor == BLACK)
		{
			if (!_pReplaceNode->m_pRight  || _pReplaceNode->m_pRight->m_nColor == BLACK)
				erase_fixup(_pReplaceNode->m_pRight, _pReplaceNode);
			else
				_pReplaceNode->m_pRight->m_nColor = BLACK;
		}
	}
	else
	{
		// 保存一些后面需要用到的变量
		int _nColor = _pReplaceNode->m_nColor;
		RBNode* _pRepalceParent = _pReplaceNode->m_pParent;
		RBNode* _pNewReplace = _pReplaceNode->m_pRight;

		transplant(_pReplaceNode, _pReplaceNode->m_pRight);

		_pReplaceNode->m_pLeft = pNode_->m_pLeft;
		pNode_->m_pLeft->m_pParent = _pReplaceNode;
		_pReplaceNode->m_pRight = pNode_->m_pRight;
		pNode_->m_pRight->m_pParent = _pReplaceNode;
		_pReplaceNode->m_nColor = pNode_->m_nColor;
		transplant(_pReplaceNode, pNode_);

		if (_nColor == BLACK)
		{
			if (!_pNewReplace || _pNewReplace->m_nColor == BLACK)
				erase_fixup(_pNewReplace, _pRepalceParent);
			else
				_pNewReplace->m_nColor = BLACK;
		}
	}

	delete pNode_;
	return true;
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

// 空结点表示为黑色
//  情况一：父结点为红色，兄弟结点为黑色，兄弟结点的子结点全为黑色
//                r                            b
//              /   \          左旋r         /   \
// 方法一:    bb     b       ======>        r     b
//                  /  \                  /  \
//                 b    b                (b)  b
//
//
//                  r                               b
//                /   \         改变颜色          /   \
// 方法二：      bb    b     ============>      (b)    r
//                   /   \                           /   \
//                  b     b                         b     b
//
// 能改变颜色就不选择进行旋转，所以选择方法二，改变颜色。
//
//  情况二：父结点为红色，兄弟结点为黑色，兄弟结点的左子结点为黑色, 右子结点为红色
//         r                                      b                         r
//       /   \               左旋               /   \    可选:改变颜色：  /   \
//      bb     b        ============>          r     r   ==========>     b     b
//           /  \                            /   \                      /   \
//          b    r                         (b)    b                    (b)   b
//
//
//  情况三：父结点为红色，兄弟结点为黑色，兄弟结点的左子结点为红色, 右子结点为黑色
//         r                        r                              r
//       /   \         右旋       /   \         改变颜色         /   \
//      bb    b      ========>   bb    r        ========>       bb    b
//          /  \                     /   \                          /   \
//         r    b                   黑    b                        黑    r
//       /  \                           /  \                           /  \
//      黑  黑                         黑   b                         黑   b
//
//  这此， 到了情况二，利用情况二来解决。
//
// 情况四：父结点为红色，兄弟结点为黑色，兄弟结点左右子结点全部为红色
//           r                        b                             r
//         /   \         左旋       /   \        改变颜色         /   \
//        bb    b      =======>    r     r      =========>       b     b
//             /  \              /  \                          /  \
//            r    r           (b)    r                      (b)   r
//
// 情况五：父结点为黑色， 兄弟结点为红色时，兄弟结点的左右子结点肯定是黑色
//        b                        r                           b
//      /   \       左旋         /   \       改变颜色        /   \
//     bb    r     ======>      b     b    =========>       r     b
//         /   \              /   \                       /   \
//        b     b            bb    b                     bb    b
//        
// 这时，bb变成父结点变成了红色，所以递归去判断。
//
// 情况六：父结点为黑色， 兄弟结点为黑色时，兄弟结点的左右子结点为红色时
//        b                         b                       b
//      /   \         左旋        /   \    改变颜色       /    \
//     bb    b      =======>     b     r   ========>     b      b
//         /   \               /   \                   /   \
//        r     r             (b)   r                 (b)   r
//
//
// 情况七：父结点为黑色， 兄弟结点为黑色时，兄弟结点的左右子结点为黑色时
//        b                         b                         bb
//      /   \       改变颜色      /   \        更新双b      /    \
//     bb    b      =======>     bb    r     =========>    b      r
//         /   \                     /   \                      /   \
//        b     b                   b     b                    b     b 
//
//  这时，对双b沿树向上继续检测。
//
// 情况八：父结点为黑色， 兄弟结点为红色时，兄弟结点的左右子结点肯定是黑色
//        b                          b                          b
//      /   \         左旋         /   \      改变颜色        /    \
//     bb    b     =========>     b     r    =========>      b      b
//         /   \                /   \                      /   \
//        b     r              (b)   b                    (b)   b
//
// 情况九：父结点为黑色， 兄弟结点为红色时，兄弟结点的左右子结点肯定是黑色
//        b                         b                              b
//      /   \         右旋        /   \         改变颜色         /   \
//     bb    b     ========>     bb    r       =========>       bb     b
//         /   \                     /   \                           /   \
//        r     b                   黑    b                         黑    r
//      /  \                            /   \                           /   \
//     黑   黑                         黑    b                         黑    b
//
//
// 对于上面的9种情况，它们有一些相同的操作，我们可以进行合并：
//
//  a. 情况一：父结点为红色，兄弟结点为黑色，兄弟结点的子结点全为黑色
//                  r                               b
//                /   \         改变颜色          /   \
//               bb    b     ============>      (b)    r
//                   /   \                           /   \
//                  b     b                         b     b
//
// b. 情况七：父结点为黑色， 兄弟结点为黑色时，兄弟结点的左右子结点为黑色时
//        b                         b                         bb
//      /   \       改变颜色      /   \        更新双b      /    \
//     bb    b      =======>     bb    r     =========>    b      r
//         /   \                     /   \                      /   \
//        b     b                   b     b                    b     b 
//
// c. 情况五：父结点为黑色， 兄弟结点为红色时，兄弟结点的左右子结点肯定是黑色
//        b                        r                           b
//      /   \       左旋         /   \       改变颜色        /   \
//     bb    r     ======>      b     b    =========>       r     b
//         /   \              /   \                       /   \
//        b     b            bb    b                     bb    b
//
// d. 情况三和情况九的操作是相同的：
//  情况三：父结点为红色，兄弟结点为黑色，兄弟结点的左子结点为红色, 右子结点为黑色
//         r                        r                              r
//       /   \         右旋       /   \         改变颜色         /   \
//      bb    b      ========>   bb    r        ========>       bb    b
//          /  \                     /   \                          /   \
//         r    b                   黑    b                        黑    r
//       /  \                           /  \                           /  \
//      黑  黑                         黑   b                         黑   b
//
// 情况九：父结点为黑色， 兄弟结点为红色时，兄弟结点的左右子结点肯定是黑色
//        b                         b                              b
//      /   \         右旋        /   \         改变颜色         /   \
//     bb    b     ========>     bb    r       =========>       bb     b
//         /   \                     /   \                           /   \
//        r     b                   黑    b                         黑    r
//      /  \                            /   \                           /   \
//     黑   黑                         黑    b                         黑    b
//
//  总结为：
//       任意                           任意                            任意
//      /    \         右旋            /    \         改变颜色         /    \
//     bb     b      ========>        bb     r       ========>        bb     b
//          /   \                          /   \                           /   \
//         r     b                              b                               r
//  此时，转换到 e去解决。
//
// e. 情况二/情况四/情况六/情况八的操作是相同的:
//  情况二：父结点为红色，兄弟结点为黑色，兄弟结点的左子结点为黑色, 右子结点为红色
//         r                                      b                         r
//       /   \               左旋               /   \    可选:改变颜色：  /   \
//      bb     b        ============>          r     r   ==========>     b     b
//           /  \                            /   \                      /   \
//          b    r                         (b)    b                    (b)   b
//
// 情况八：父结点为黑色， 兄弟结点为红色时，兄弟结点的左右子结点肯定是黑色
//        b                          b                          b
//      /   \         左旋         /   \      改变颜色        /    \
//     bb    b     =========>     b     r    =========>      b      b
//         /   \                /   \                      /   \
//        b     r              (b)   b                    (b)   b
//
// 情况四：父结点为红色，兄弟结点为黑色，兄弟结点左右子结点全部为红色
//           r                        b                             r
//         /   \         左旋       /   \        改变颜色         /   \
//        bb    b      =======>    r     r      =========>       b     b
//             /  \              /  \                          /  \
//            r    r           (b)    r                      (b)   r
//
// 情况六：父结点为黑色， 兄弟结点为黑色时，兄弟结点的左右子结点为红色时
//        b                         b                       b
//      /   \         左旋        /   \    改变颜色       /    \
//     bb    b      =======>     b     r   ========>     b      b
//         /   \               /   \                   /   \
//        r     r             (b)   r                 (b)   r
//
// 总结为：
//      任意                        b                             任意 
//     /   \         左旋         /   \       改变颜色           /    \
//    bb    b      =======>     任意   r    ==========>         b      b
//        /   \                /   \                          /  \
//      任意   r              bb    任意                     (b)  任意
//
//
// 删除之后调整红黑树，使其符合红黑树的性质
// 写红黑树时，真的需要辅助结点，否则真不好弄. 因为把空结点当作黑结点来处理很方便，要不然
// 的话，你需要写很多判断为空时的代码, 还需要把父结点传进来。
// 下面就是没有使用辅助结点的做法。 头皮发麻！！！
void RBTree::erase_fixup(RBNode* pNode_, RBNode* pParent_)
{
	// 父结点的参数为空时，直接返回, 当前结点的可能为空。
	if (!pParent_)
		return;

	// 当前结点要不么空，要不为黑结点
	assert(!pNode_ || pNode_->m_nColor == BLACK);

	while (true)
	{
		if (pNode_ == pParent_->m_pLeft)
		{
			RBNode* _pSibling= pParent_->m_pRight;
			// 情况a:
			if (pParent_->m_nColor == RED &&		// 父结点为红色
					_pSibling->m_nColor == BLACK && // 兄弟结点为黑
					(!_pSibling->m_pLeft && !_pSibling->m_pRight ||		// 兄弟结点都为空或都为黑
					_pSibling->m_pLeft->m_nColor == BLACK && _pSibling->m_pRight->m_nColor == BLACK))
			{
				_pSibling->m_nColor = RED;
				pParent_->m_nColor = BLACK;
				break;
			}

			// 情况b:
			if (pParent_->m_nColor == BLACK &&		// 父结点为黑色
					_pSibling->m_nColor == BLACK && // 兄弟结点为黑
					(!_pSibling->m_pLeft && !_pSibling->m_pRight ||		// 兄弟结点都为空或都为黑
					_pSibling->m_pLeft->m_nColor == BLACK && _pSibling->m_pRight->m_nColor == BLACK))
			{
				_pSibling->m_nColor = RED;
				pNode_ = pParent_;
				pParent_ = pNode_->m_pParent;
				if (!pParent_)		// 此时对应了pNode_此时为根结点的情况。
					break;
				else
					continue;
			}

			// 情况 c:
			if ( _pSibling->m_nColor == RED && // 兄弟结点为红
					(!_pSibling->m_pLeft && !_pSibling->m_pRight ||		// 兄弟结点都为空或都为黑, 其中兄弟结点肯定不为空
					_pSibling->m_pLeft->m_nColor == BLACK && _pSibling->m_pRight->m_nColor == BLACK))
			{
				left_rotate(pParent_);
				pParent_->m_nColor = RED;
				_pSibling->m_nColor = BLACK;
				continue;
			}

			// 情况 d:
			if ( _pSibling->m_nColor == BLACK && // 兄弟结点为黑
					_pSibling->m_pLeft->m_nColor == RED && 	// 兄弟的左子结点为红
					(!_pSibling->m_pRight || _pSibling->m_pRight->m_nColor == BLACK))	// 兄弟的右子结点为空或黑
			{
				// 先改变颜色再旋转，要不找不到了。
				_pSibling->m_nColor = RED;
				_pSibling->m_pLeft->m_nColor = BLACK;
				right_rotate(_pSibling);
			}

			// 不需要判断了，就剩下情况e了。
			// 改变颜色，然后进行左旋
			_pSibling->m_nColor = pParent_->m_nColor;
			pParent_->m_nColor = BLACK;
			_pSibling->m_pRight->m_nColor = BLACK;
			left_rotate(pParent_);
			break;
		}
		else     // 为右结点时， 是对偶的, 直接全局替换了。
		{
			RBNode* _pSibling= pParent_->m_pLeft;
			// 情况a:
			if (pParent_->m_nColor == RED &&		// 父结点为红色
					_pSibling->m_nColor == BLACK && // 兄弟结点为黑
					(!_pSibling->m_pRight && !_pSibling->m_pLeft ||		// 兄弟结点都为空或都为黑
					_pSibling->m_pRight->m_nColor == BLACK && _pSibling->m_pLeft->m_nColor == BLACK))
			{
				_pSibling->m_nColor = RED;
				pParent_->m_nColor = BLACK;
				break;
			}

			// 情况b:
			if (pParent_->m_nColor == BLACK &&		// 父结点为黑色
					_pSibling->m_nColor == BLACK && // 兄弟结点为黑
					(!_pSibling->m_pRight && !_pSibling->m_pLeft ||		// 兄弟结点都为空或都为黑
					_pSibling->m_pRight->m_nColor == BLACK && _pSibling->m_pLeft->m_nColor == BLACK))
			{
				_pSibling->m_nColor = RED;
				pNode_ = pParent_;
				pParent_ = pNode_->m_pParent;
				if (!pParent_)		// 此时对应了pNode_此时为根结点的情况。
					break;
				else
					continue;
			}

			// 情况 c:
			if ( _pSibling->m_nColor == RED && // 兄弟结点为红
					(!_pSibling->m_pRight && !_pSibling->m_pLeft ||		// 兄弟结点都为空或都为黑, 其中兄弟结点肯定不为空
					_pSibling->m_pRight->m_nColor == BLACK && _pSibling->m_pLeft->m_nColor == BLACK))
			{
				right_rotate(pParent_);
				pParent_->m_nColor = RED;
				_pSibling->m_nColor = BLACK;
				continue;
			}

			// 情况 d:
			if ( _pSibling->m_nColor == BLACK && // 兄弟结点为黑
					_pSibling->m_pRight->m_nColor == RED && 	// 兄弟的右子结点为红
					(!_pSibling->m_pLeft || _pSibling->m_pLeft->m_nColor == BLACK))	// 兄弟的左子结点为空或黑
			{
				// 先改变颜色再旋转，要不找不到了。
				_pSibling->m_nColor = RED;
				_pSibling->m_pRight->m_nColor = BLACK;
				left_rotate(_pSibling);
			}

			// 不需要判断了，就剩下情况e了。
			// 改变颜色，然后进行右旋
			_pSibling->m_nColor = pParent_->m_nColor;
			pParent_->m_nColor = BLACK;
			_pSibling->m_pLeft->m_nColor = BLACK;
			right_rotate(pParent_);
			break;
		}
	}
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
