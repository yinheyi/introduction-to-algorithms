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
*   Created Time: 2019年05月08日 星期三 21时53分25秒
*   Modifed Time: 2019年05月09日 星期四 00时15分17秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/


// 对于冒泡排序，这肯定是大家接触编程时第一个碰到的排序算法。
// 原理很简单: 以从小到大排序为例，假设一个数组的长度为n, 则：
// 第一次: 从数组尾部开始向前, 两两元素之间进行比较， 共比较n-1次，就可以把最小元素移
// 动到数组下标为0的地方, 此时有1个排序完成, 剩余n-1个还没有排序。
// 第二次：还是从数组尾部开始向前，两两元素之间进行比较， 共比较n-2次，就可以把剩余的
// 元素中最小的元素移动到数组下标为1的地方，此时有2个元素排序完成，剩余n-2还没有排序。
// 第三次: 重复以上过程。
//
//    原始          第一次       第二次        第三次      第四次      第五次
//     3             -12          -12           -12         ...         ...
//     2              3            1             1
//     8              2            3             1
//     1              8            2             3
//    -12             1            8             2
//     32             1            1             8
//     1              32           32           32
//
//
// 说明：1. 冒泡排序是稳定排序，只有当两个元素不同时才会交换；
//       2. 冒泡排序为原址排序，不需要借额外的空间；
//       3. 冒泡排序通常见到的都是通过循环来实现的，其实通过递归来实现更简洁。 
//       4. 冒泡排序的时间复杂度为O(N*N)
//
//
bool less(int lhs, int rhs);
bool greate(int lhs, int rhs);
static inline void swap(int& lhs, int & rhs);
void PrintArray(int array[], int nLength_);
typedef bool (*Comp)(int, int);

//  基于循环来实现的冒泡排序:
void BubbleSort_Loop(int array[], int nLength_, Comp CompFunc)
{
	if (array == nullptr || nLength_ <= 1 || CompFunc == nullptr)
		return;

	// 对于n个元素，只需要排前n-1个元素即可, 即下标为0, 1, 2, ..., n-2的元素。
	for (int i = 0; i < nLength_ - 1; ++i)
	{
		// 如果要使下标为i的元素变成有序的，需要从数组尾部开始两两交换，直至交换到i
		for (int j = nLength_ - 1; j > i; --j)
		{
			if (!CompFunc(array[j-1], array[j]))
			{
				swap(array[j-1], array[j]);
			}
		}
	}
}

// 基于递归来实现冒泡排序：
void BubbleSort_Recursion(int array[], int nLength_, Comp CompFunc)
{
	if (array == nullptr || nLength_ <= 1 || CompFunc == nullptr)
		return;

	// 从数组尾部向前，对不符合要求的元素进行两两交换，从而使数组头部的元素为最小或最大
	for (int i = nLength_ - 1; i > 0;  --i)
	{
		if (!CompFunc(array[i-1], array[i]))
		{
			swap(array[i-1], array[i]);
		}
	}

	// 对数组剩余的元素进行递归操作
	BubbleSort_Recursion(array + 1, nLength_ - 1, CompFunc);
}

// 小小的测试
#include <iostream>
/***************    main.c     *********************/
int main(int argc, char* argv[])
{
	int test1[10] = {-1, -23, 33, 423, -2349, 1, 1, 0, 9, 10};
	std::cout << "原顺序为：" << std::endl;
	PrintArray(test1, 10);

	std::cout << "基于循环的从小到大排序：" << std::endl;
	BubbleSort_Loop(test1, 10, less);
	PrintArray(test1, 10);
	std::cout << "基于循环的从大到小排序：" << std::endl;
	BubbleSort_Loop(test1, 10, greate);
	PrintArray(test1, 10);

	std::cout << "基于递归的从小到大排序：" << std::endl;
	BubbleSort_Recursion(test1, 10, less);
	PrintArray(test1, 10);
	std::cout << "基于递归的从大到小排序：" << std::endl;
	BubbleSort_Recursion(test1, 10, greate);
	PrintArray(test1, 10);

	return 0;
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

// 交换两个元素的值
static inline void swap(int& lhs, int & rhs)
{
	int _nTemp = lhs;
	lhs = rhs;
	rhs = _nTemp;
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

