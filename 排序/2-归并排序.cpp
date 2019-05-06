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
*   Created Time: 2019年05月06日 星期一 22时22分57秒
*   Modifed Time: 2019年05月06日 星期一 23时46分55秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/


// 归并排序，分治法的典型代表: 将原问题分解了几个子问题，解决子问题，再合并子问题的解，
// 这样就得到了原问题的解。
// 分治本质上就是把原问题分解为几个子问题来解决。
// 快速排序也是分治思想来解决。
//
//
// 归并排序(merge-sort):
// 1. 把一个待排序的数组分解为两个子数组；
// 2. 对两个子数组进行排序（通过递归地调用自己来实现）;
// 3. 对两个已经排序的数组进行合并。
//
// 分析：
// 1. 一个数组一直分解下去，只到分解成只包含一个元素的子数组为止, 此时自然是有序的；
// 2. 归并排序的重点在于合并，而不是对子数组的排序。（快速排序与它恰恰相反，快速排序的
// 重点是对子数组进行排序，而不是合并，因为它不需要合并了）
//
//
#include <cstring>
#include <iostream>
typedef bool(*CompareFunc)(int, int);

// 下面函数实现合并功能，输入三个下标参数表示了两个子数组, :[nStart_, nMiddle)和[nMiddle, nEnd)
void Merge(int array[], int nStart_, int nMiddle_, int nEnd_, CompareFunc comp)
{
	if (array == nullptr || nStart_ >= nMiddle_ || nMiddle_ >= nEnd_)
		return;

	int* _pArrayBegin = array + nStart_;		// 数组的起始位置的指针，后面会使用到。
	int _nCount = nEnd_ - nStart_;
	// 建立一个临时数组存放中间数据
	int _nIndex = 0;
	int* _pTempArray = new int[nEnd_ - nStart_]{0};

	// 对两个子数组进行合并
	while (nStart_ < nMiddle_ && nMiddle_ < nEnd_)
	{
		if (comp(array[nStart_],  array[nMiddle_]))
		{
			_pTempArray[_nIndex] = array[nStart_];
			++nStart_;
		}
		else
		{
			_pTempArray[_nIndex] = array[nMiddle_];
			++nMiddle_;
		}
		++_nIndex;
	}

	// 把不为空的子数组的元素追加到临时数组中去（都为空时，也没有问题）
	if (nStart_ < nMiddle_)
		memcpy(_pTempArray + _nIndex, array + nStart_, sizeof(int) * (nMiddle_ - nStart_));
	else
		memcpy(_pTempArray + _nIndex, array + nMiddle_, sizeof(int) * (nEnd_ - nMiddle_));

	// 数据交换
	memcpy(_pArrayBegin, _pTempArray, sizeof(int) * _nCount);

	delete [] _pTempArray;
	_pTempArray = nullptr;
}


// 归并排序功能实现函数
void MergeSort(int array[], int nStart_, int nEnd_, CompareFunc comp)
{
	// 数组指针为空，或者数组内的个数少于等于1个时，直接返回。
	if (nullptr == array ||  (nEnd_ - nStart_) <= 1)
		return;

	// 划分为两个子数组并递归调用自身进行排序
	int _nMiddle = (nStart_ + nEnd_) / 2;
	MergeSort(array, nStart_, _nMiddle, comp);
	MergeSort(array, _nMiddle, nEnd_, comp);

	// 合并排序完成的子数组
	Merge(array, nStart_, _nMiddle, nEnd_, comp);
}


// 比较函数
bool less(int lhs, int rhs)
{
	return lhs < rhs;
}


// 打印数组函数
void PrintArray(int array[], int nLength_)
{
	if (nullptr == array || nLength_ <= 0)
		return;

	for (int i = 0; i < nLength_; ++i)
	{
		std::cout << array[i] << " ";
	}

	std::cout << std::endl;
}



/***************    main.c     *********************/
int main(int argc, char* argv[])
{
	int array[10] = {1, 3, -1, -123, 53, 1, 111232, -1, 13134, 1};
	PrintArray(array, 10);

	MergeSort(array, 0, 10, less);

	PrintArray(array, 10);

	return 0;
}

