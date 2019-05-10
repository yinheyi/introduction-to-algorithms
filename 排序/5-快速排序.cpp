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
*   Created Time: 2019年05月08日 星期三 21时54分04秒
*   Modifed Time: 2019年05月10日 星期五 22时16分17秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/


// 1. 快速排序是分治思想的又一典型代表，是应用最广的排序算法。
// 2. 分治思想就是把原问题的解分解为两个或多个子问题解，求解出子问题的解之后再构造出原
// 问题的解。
// 3. 在快速排序算法中，它的思想是把一个待排序的数组分成前半部分和后半部分，并且要求
// 前半部分的值都大于等于或都小于等于后半部分的解, 当前半部分与后半部分都变成有序（通
// 过递归调用快速排序来实现）后，我们就不需要合并两个子问题的解就已经得到了原问题的解。
// 这也是为什么要求前半部分都大于等于或都小于等于后半部分的原因。
// 4.所以呢，快速排序的核心在于如何把一个待排序的数组分成两部分！
//
// 核心点：
// 1. 如何把待排序的数组划分为符合要求的两部分！
// 2. 期望的时间复杂度为O(NlogN), 最坏的时间复杂度为O(N*N)
// 3. 快速排序为原址排序，不需要额外的内存空间.
// 4. 快速排序不是稳定排序, 在交换过程中会破坏稳定性。
//
#include<cassert>
#include <stdexcept>
#include <iostream>
static inline void swap(int&, int&);
bool less(int lhs, int rhs);
bool greate(int lhs, int rhs);
static void PrintArray(int array[], int nLength_);
typedef bool (*Compare)(int, int);

/****************  版本一：使用数组的长度作为参数        ***************/
// 该函数实现对数组数列的划分;
// 输入值为数组指针/数组的长度/比较函数指针，
// 返回值为划分点的下标, 也就是后半部分第一个元素的下标;
int Partition(int array[], int nLength_, Compare CompFunc)
{
	if (array == nullptr || nLength_ <= 0 || CompFunc == nullptr)
	{
		assert(false);
		throw std::invalid_argument("参数不合法！");
	}

	int _nBoundValue = array[0];		// 划分区间的边界值
	int _nBoundIndex = 0;				// 指向边界的下标, 即第二部分第一个元素的下标;
	for (int i = 1; i < nLength_; ++i)
	{
		if (CompFunc(array[i], _nBoundValue))
		{
			swap(array[i], array[_nBoundIndex]);
			++_nBoundIndex;
		}
	}

	// 如果第一个元素正好是最大或最小元素时，把返回值加1, 也就是把数组划分为第一个元素
	// 和剩余的其它元素两部分。
	if (0 == _nBoundIndex)
		return _nBoundIndex + 1;
	else
		return _nBoundIndex;
}

// 快速排序的功能函数
void QuickSort(int array[], int nLength_, Compare CompFunc)
{
	if (array == nullptr || nLength_ <=1 || CompFunc == nullptr)
		return;

	int _nPartionIndex = Partition(array, nLength_, CompFunc);
	QuickSort(array, _nPartionIndex, CompFunc);
	QuickSort(array + _nPartionIndex, nLength_ - _nPartionIndex, CompFunc);
}

/****************  版本二：使用数组的下标区间作为参数        ***************/
// 该函数实现对数组的划分。
// 输入参数为数组指针/半闭半开区间[start, end)表示的数组范围/比较谓词
// 返回值为划分点的下标, 也即后半部分第一个元素的下标。
int Partition_Version2(int array[], int nStart_, int nEnd_, Compare CompFunc)
{
	if (array == nullptr || nEnd_ - nStart_ <= 0 || CompFunc == nullptr)
	{
		assert(false);
		throw std::invalid_argument("参数不合法！");
	}

	int _nBoundValue = array[nStart_];		// 划分区间的边界值
	int _nBoundIndex = nStart_;				// 指向边界的下标, 即第二部分第一个元素的下标;
	for (int i = nStart_ + 1; i < nEnd_; ++i)
	{
		if (CompFunc(array[i], _nBoundValue))
		{
			swap(array[i], array[_nBoundIndex]);
			++_nBoundIndex;
		}
	}

	// 如果第一个元素正好是最大或最小元素时，把返回值加1, 也就是把数组划分为第一个元素
	// 和剩余的其它元素两部分。
	if (_nBoundIndex == nStart_)
		return _nBoundIndex + 1;
	else
		return _nBoundIndex;
}

void QuickSort_Version2(int array[], int nStart_, int nEnd_, Compare CompFunc)
{
	if (array == nullptr || nEnd_ - nStart_ <= 1 || CompFunc ==nullptr)
		return;

	int _nPartionIndex = Partition_Version2(array, nStart_, nEnd_, CompFunc);
	QuickSort_Version2(array, nStart_, _nPartionIndex, CompFunc);
	QuickSort_Version2(array, _nPartionIndex, nEnd_, CompFunc);
}

// 测试函数
/***************    main.c     *********************/
int main(int argc, char* argv[])
{
	int array[10] = {-12, 23, 443, 112, 12, -9098, 3432, 0, 0, 0};
	std::cout << "原数组的顺序为：" << std::endl;
	PrintArray(array, 10);
	std::cout << "版本一的快速排序：" << std::endl;
	std::cout << "从小到大：" << std::endl;
	QuickSort(array, 10, less);
	PrintArray(array, 10);
	std::cout << "从大到小：" << std::endl;
	QuickSort(array, 10, greate);
	PrintArray(array, 10);
	std::cout << std::endl;


	int array2[10] = {-12, 23, 443, 112, 12, -9098, 3432, 0, 0, 0};
	std::cout << "版本二的快速排序：" << std::endl;
	std::cout << "从小到大：" << std::endl;
	QuickSort_Version2(array2, 0, 10, less);
	PrintArray(array2, 10);
	std::cout << "从大到小：" << std::endl;
	QuickSort_Version2(array2, 0, 10, greate);
	PrintArray(array2, 10);

	return 0;
}


inline void swap(int& lhs, int& rhs)
{
	int _nTemp = lhs;
	lhs = rhs;
	rhs = _nTemp;
}

// 小于比较函数
bool less(int lhs, int rhs)
{
	return lhs < rhs;
}

// 大于比较函数
bool greate(int lhs, int rhs)
{
	return lhs > rhs;
}

// 打印数组函数
static void PrintArray(int array[], int nLength_)
{
	if (nullptr == array || nLength_ <= 0)
		return;

	for (int i = 0; i < nLength_; ++i)
	{
		std::cout << array[i] << " ";
	}

	std::cout << std::endl;
}

