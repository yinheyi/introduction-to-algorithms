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
*   Created Time: 2019年05月13日 星期一 23时21分43秒
*   Modifed Time: 2019年05月16日 星期四 00时23分35秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/
#include <iostream>
#include <vector>
#include <stdexcept>

// 1. 最大堆或最小堆是使用一个数组表示的完全二叉树，除了最底层之外，其它层都为满的，并且最
// 底层从左到右填充。在最大堆中，父结点大于或等于左右子结点，在最小堆中，父结点小于或等
// 于左右子结点。
// 2. 用于表示堆的数组的下标从0开始, 对应了完全二叉树的分层遍历的节点, 如下图所示。通过图中
// 特点可以知道：
//     当父结点的下标为i时，左子结点对应的下标为：2i+1, 右子结点对应的下标为：2i+2;
//     当子结点的下标为j时，对应的父结点的下标为：(j-1)/2.
// 所以，我们可以很方便地定义一个宏，使用移位操作来获取父结点或子结点的下标。
//                    0
//                /       \
//             1             2
//           /   \         /    \
//         3      4       5       6
//        / \    / \     /  \   /   \
//      7    8  9  10  11   12 13   14
//
// 3. 最大堆/最小堆可以具有如下的方法：
//    * 建立一个最大堆或最小堆 
//    * 添加元素
//    * 获取最大堆的最大值或最小堆的最小值
//    * 删除最大堆的最大值或删除最小堆的最小值
//    * 修改堆中元素的值。
//
// 4. 在堆的所有操作中，最关键的在于如何维持最大堆或最小堆的性质。即假设一个结点的左子树和
// 右子树表示的都满足最大堆或最小堆的性质，而该结点不满足, 也就是对于最大堆来说，该结点
// 不满足大于等于左右子结点的性质，对于最小堆来说，该结点不满足小于或等于左右子结点的性
// 质。 我们需要调整结点的位置使其能够满足最小堆或最小堆的性质。
//
//
#define LEFT(i) (((i) << 1) + 1)
#define RIGHT(i) (((i) + 1) << 1)
#define PARENT(i) (((i)-1) >> 1)
/****************    最小堆          **********/
class MinHeap
{
public:
	void Insert(int nValue_);
	void PopMin();
	void Modify(size_t nIndex_, int nValue_);
	int Minimum() const;
	bool empty() const;
	size_t size() const;
private:
	void Heapify(size_t nIndex_);		// 维护堆的性质
	std::vector<int> m_vecArray;	// 底层使用std::vector,方便增加元素
};

void MinHeap::Insert(int nValue_)
{
}

void MinHeap::PopMin()
{
}

void MinHeap::Modify(size_t nIndex_, int nValue_)
{
}

int MinHeap::Minimum() const
{
	if (m_vecArray.empty())
		throw std::out_of_range("最小堆元素为空，无最小值。");
	else
		return m_vecArray.front();
}

bool MinHeap::empty() const
{
	return m_vecArray.empty();
}

size_t MinHeap::size() const
{
	return m_vecArray.size();
}

void MinHeap::Heapify(size_t nIndex_)
{
	if (nIndex_ < 0 || nIndex_ >= m_vecArray.size() || m_vecArray.empty())
		return;

	// 获取最大值的下标
	size_t _nMaxIndex = nIndex_;
	if (LEFT(nIndex_) < m_vecArray.size() && m_vecArray[LEFT(nIndex_)] > m_vecArray[_nMaxIndex])
		_nMaxIndex = LEFT(nIndex_);
	if (RIGHT(nIndex_) < m_vecArray.size() && m_vecArray[RIGHT(nIndex_)] > m_vecArray[_nMaxIndex])
		_nMaxIndex = RIGHT(nIndex_);

	if (_nMaxIndex != nIndex_)
	{
		std::swap(m_vecArray[nIndex_], m_vecArray[_nMaxIndex]);
		Heapify(_nMaxIndex);
	}
}
//
//
/**********************    测试程序     *************************/
int main(int argc, char* argv[])
{
	return 0;
}

