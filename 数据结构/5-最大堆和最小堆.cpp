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
*   Modifed Time: 2019年05月16日 星期四 23时22分05秒
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
	m_vecArray.push_back(nValue_);
	size_t _nCurrentIndex = m_vecArray.size() - 1;
	while (_nCurrentIndex > 0 && m_vecArray[PARENT(_nCurrentIndex)] > m_vecArray[_nCurrentIndex])
	{
		std::swap(m_vecArray[PARENT(_nCurrentIndex)], m_vecArray[_nCurrentIndex]);
		_nCurrentIndex = PARENT(_nCurrentIndex);
	}
}

void MinHeap::PopMin()
{
	if (m_vecArray.empty())
	{
		std::cerr << "最小堆为空， 删除最小值失败。" << std::endl;
		return;
	}

	std::swap(m_vecArray[0], m_vecArray[m_vecArray.size() - 1]);
	m_vecArray.pop_back();
	Heapify(0);
}

void MinHeap::Modify(size_t nIndex_, int nValue_)
{
	// 分两种情况:如果修改后的值大于修改之前的值，则需要维护该结点的堆性质 ; 如果修改之后的值
	// 小于修改之前的值，则需要沿着二叉树向上处理它的父结点，类似于插入值的代码
	int _nOldValue = m_vecArray[nIndex_];
	m_vecArray[nIndex_] = nValue_;
	if (nValue_ > _nOldValue)
	{
		Heapify(nIndex_);
	}
	else
	{
		while (nIndex_ > 0 && m_vecArray[PARENT(nIndex_)] > m_vecArray[nIndex_])
		{
			std::swap(m_vecArray[PARENT(nIndex_)], m_vecArray[nIndex_]);
			nIndex_ = PARENT(nIndex_);
		}
	}
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

	// 获取最小值的下标
	size_t _nMaxIndex = nIndex_;
	if (LEFT(nIndex_) < m_vecArray.size() && m_vecArray[LEFT(nIndex_)] < m_vecArray[_nMaxIndex])
		_nMaxIndex = LEFT(nIndex_);
	if (RIGHT(nIndex_) < m_vecArray.size() && m_vecArray[RIGHT(nIndex_)] < m_vecArray[_nMaxIndex])
		_nMaxIndex = RIGHT(nIndex_);

	if (_nMaxIndex != nIndex_)
	{
		std::swap(m_vecArray[nIndex_], m_vecArray[_nMaxIndex]);
		Heapify(_nMaxIndex);
	}
}
/*************************  最大堆    **************************/
// 有了最小堆的实现，最大堆也就不难了
class MaxHeap
{
public:
	void Insert(int value_);
	void PopMaximum();
	void ModifyValue(size_t nIndex_, int nValue_);
	int Maximum() const;
	bool empty() const;
	size_t size() const;
private:
	void MaxHeapify(size_t nIndex_);
	std::vector<int> m_vecArray;
};

void MaxHeap::Insert(int value_)
{
	m_vecArray.push_back(value_);
	size_t _nCurrentIndex = m_vecArray.size() - 1;
	while (_nCurrentIndex > 0 && m_vecArray[_nCurrentIndex] > m_vecArray[PARENT(_nCurrentIndex)])
	{
		std::swap(m_vecArray[_nCurrentIndex], m_vecArray[PARENT(_nCurrentIndex)]);
		_nCurrentIndex = PARENT(_nCurrentIndex);
	}
}

void MaxHeap::PopMaximum()
{
	if (m_vecArray.empty())
	{
		std::cout << "最大堆中为空，无法删除最大值." << std::endl;
		return;
	}

	std::swap(m_vecArray[0], m_vecArray[m_vecArray.size() - 1]);
	m_vecArray.pop_back();
	MaxHeapify(0);
}

void MaxHeap::ModifyValue(size_t nIndex_, int nValue_)
{
	if (nIndex_ >= m_vecArray.size() || nIndex_ < 0)
	{
		std::cerr << " 给定的下标不正确!" << std::endl;
	}

	// 修改给定下标的值
	int _nOldValue = m_vecArray[nIndex_];
	m_vecArray[nIndex_] = nValue_;

	// 维护最大堆的性质
	if (nValue_ < _nOldValue)
	{
		MaxHeapify(nIndex_);
	}
	else
	{
		while (nIndex_ > 0 && m_vecArray[nIndex_] > m_vecArray[PARENT(nIndex_)])
		{
			std::swap(m_vecArray[nIndex_], m_vecArray[PARENT(nIndex_)]);
			nIndex_ = PARENT(nIndex_);
		}
	}
}

int MaxHeap::Maximum() const
{
	if (m_vecArray.empty())
	{
		throw std::out_of_range("最大堆为空，不存在最大值");
	}

	return m_vecArray.front();
}

bool MaxHeap::empty() const
{
	return m_vecArray.empty();
}


size_t MaxHeap::size() const
{
	return m_vecArray.size();
}

void MaxHeap::MaxHeapify(size_t nIndex_)
{
	if (nIndex_ < 0 || nIndex_ >= m_vecArray.size())
		return;

	size_t _nMaxIndex = nIndex_;
	if (LEFT(nIndex_) < m_vecArray.size() && m_vecArray[LEFT(nIndex_)] > m_vecArray[_nMaxIndex])
		_nMaxIndex = LEFT(nIndex_);
	if (RIGHT(nIndex_) < m_vecArray.size() && m_vecArray[RIGHT(nIndex_)] > m_vecArray[_nMaxIndex])
		_nMaxIndex = RIGHT(nIndex_);

	if (_nMaxIndex != nIndex_)
	{
		std::swap(m_vecArray[_nMaxIndex], m_vecArray[nIndex_]);
		MaxHeapify(_nMaxIndex);
	}
}

/**********************    测试程序     *************************/

int main(int argc, char* argv[])
{
	// 最小堆测试代码
	MinHeap _minHeap;
	std::cout << "向最小堆中插入10,9,8,7,6,5,4,3,2,1" << std::endl;
	_minHeap.Insert(10);
	_minHeap.Insert(9);
	_minHeap.Insert(8);
	_minHeap.Insert(7);
	_minHeap.Insert(6);
	_minHeap.Insert(5);
	_minHeap.Insert(4);
	_minHeap.Insert(3);
	_minHeap.Insert(2);
	_minHeap.Insert(1);

	std::cout << "此时，最小堆中的元素个数为： " << _minHeap.size() << std::endl;

	std::cout << "最小堆中的最小值为：" << std::endl;
	std::cout << _minHeap.Minimum() << std::endl;

	_minHeap.Modify(9, -10);
	std::cout << "把下标为9的元素修改为-10，此时最小值为："  << _minHeap.Minimum() << std::endl;

	_minHeap.PopMin();
	_minHeap.PopMin();
	_minHeap.PopMin();
	_minHeap.PopMin();
	std::cout << "从最小堆中删除了4个最小值，此时应该输出7" << std::endl;
	std::cout << _minHeap.Minimum()  << std::endl;

	// 最大堆测试代码
	MaxHeap _maxHeap;
	std::cout << "向最大堆中插入1,2,3,4,5,6,7,8,9,10" << std::endl;
	_maxHeap.Insert(1);
	_maxHeap.Insert(2);
	_maxHeap.Insert(3);
	_maxHeap.Insert(4);
	_maxHeap.Insert(5);
	_maxHeap.Insert(6);
	_maxHeap.Insert(7);
	_maxHeap.Insert(8);
	_maxHeap.Insert(9);
	_maxHeap.Insert(10);

	std::cout << "此时，最大堆中的元素个数为： " << _maxHeap.size() << std::endl;

	std::cout << "最大堆中的最大值为：" << std::endl;
	std::cout << _maxHeap.Maximum() << std::endl;

	_maxHeap.ModifyValue(9, 1000);
	std::cout << "把下标为9的元素修改为1000，此时最大值为："  << _maxHeap.Maximum() << std::endl;

	_maxHeap.PopMaximum();
	_maxHeap.PopMaximum();
	_maxHeap.PopMaximum();
	_maxHeap.PopMaximum();
	std::cout << "从最小堆中删除了4个最大值，此时应该输出7" << std::endl;
	std::cout << _maxHeap.Maximum()  << std::endl;

	return 0;
}

