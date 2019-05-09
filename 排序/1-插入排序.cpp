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
*   Created Time: 2019年05月05日 星期日 21时48分52秒
*   Modifed Time: 2019年05月09日 星期四 00时14分23秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/

#include <iostream>
// 插入排序的实现(insertion-sort)
// 思想：1. 首先从待排序的数组中选择一个数作为初始有序状态的序列；
//       2. 然后再从数组中选择下一个数，插入到有序序列中的合适位置，使新的序列也是有序的;
//       3. 不断重复这个过程......
//
// 核心点：1. 合理安排代码，使插入排序不需要额外的空间的, 进行原址排序。
//         2. 如何找到合适的插入位置，以及插入时怎么移动其它的相关数据问题。
//
// 代码如下, 该函数默认从小到大排序：
void insertion_sort(int array[], size_t nLength_)
{
	// 参数的检测
	if (array == nullptr || nLength_ < 2)
		return;

	for (size_t i = 1; i < nLength_; ++i)	// 注意：i是从1开始
	{
		int _nCurrent = array[i];		// 当前待排序的数字

		// 此时，下标为 0 ~ i-1的数字是有序的. 向后移动比当前序数字大的所有数，为该数腾出一个位置来。
		int _nLessEqualIndex = i - 1;
		while (_nLessEqualIndex >= 0 && array[_nLessEqualIndex] > _nCurrent)
		{
			array[_nLessEqualIndex + 1] = array[_nLessEqualIndex];
			--_nLessEqualIndex;
		}
		// 把新数插入到合适的位置
		array[_nLessEqualIndex + 1] = _nCurrent;
	}
}


// 该函数实现输出数组内的元素。
void PrintArray(int array[], size_t nLength_)
{
	for (size_t i = 0; i < nLength_; ++i)
	{
		std::cout << array[i] << " ";
	}
	std::cout << std::endl;
}


// 测试
/***************    main.c     *********************/
int main(int argc, char* argv[])
{
	int array[10] = {4, 1, 7, 9, 1, -2, 43, 34, 903, -23};

	std::cout << "排序前：" << std::endl;
	PrintArray(array, 10);

	insertion_sort(array, 10);

	std::cout << "排序后：" << std::endl;
	PrintArray(array, 10);

	return 0;
}

