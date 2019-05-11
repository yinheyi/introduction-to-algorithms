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
*   Created Time: 2019年05月11日 星期六 10时19分07秒
*   Modifed Time: 2019年05月11日 星期六 13时17分03秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/
#include<string.h>
#include<iostream>

// 任何比较排序算法的时间复杂度的上限为O(NlogN), 不存在比o(nlgN)最少的比较排序算法。
// 如果想要在时间复杂度上超过O(NlogN)的时间复杂度，肯定需要加入其它条件。
//
// 计数排序的核心思想(来自算法导论）：
// 对于每一个输入元素x, 确定小于等于x的个数为i。利用这一信息，就可以把元素x放到输出数组
// 的正确位置了,即把元素放到输出数组下标为i-1的位置。
//
// 重要说明：
// 1. 计数排序要求待排序的n个元素的大小在[0, k]之间，并且k与n在一个数量级上，即k=O(n).
// 此时使用计数排序可以把时间复杂度降到O(n)上。
// 2. 计数排序不是基于比较的排序算法，它基于计数策略。
// 3. 写计数排序算法时，应该把它写成稳定排序的。
//
// 计数排序代码如下：
// 参数说明：array表示数组指针，nLength_表示数组的最大长度，nMaxNumber_表示数组元素中的最大值；
void CountingSort(int array[], int nLength_, int nMaxNumber_)
{
	// 参数的合法化检测
	if (nullptr == array || nLength_ <= 1 || nMaxNumber_ <= 0)
		return;

	// 统计待排序数组中每一个元素的个数
	// 注意：此处new出来的数组的大小为nMaxNumber_ + 1, 用于统计[0, nMaxNumber_]范围内的元素
	int* ArrayCount = new int[nMaxNumber_ + 1]{0};
	for (int i = 0; i < nLength_; ++i)
	{
		++ArrayCount[array[i]];
	}

	// 此处计算待排序数组中小于等于第i个元素的个数. 
	// 备注：如果要进行大到小的排序，就计算大于等于第i个元素的个数, 也就从后向前进行累加;
	for (int i = 1; i < nMaxNumber_ + 1; ++i)
	{
		ArrayCount[i] += ArrayCount[i-1];
	}

	// 把待排序的数组放到输出数组中, 为了保持排序的稳定性，从后向前添加元素
	int* ArrayResult = new int[nLength_];
	for (int i = nLength_ - 1; i >=0; --i)
	{
		int _nIndex = ArrayCount[array[i]] - 1;	// 元素array[i]在输出数组中的下标
		ArrayResult[_nIndex] = array[i];

		// 因为可能有重复的元素，所以要减1,为下一个重复的元素计算正确的下标;
		--ArrayCount[array[i]];
	}

	// 交换数据并释放内存空间
	memcpy(array, ArrayResult, sizeof(int) * nLength_);
	delete [] ArrayCount;
	ArrayCount = nullptr;
	delete [] ArrayResult;
	ArrayResult = nullptr;
}

// 测试代码
/***************    main.c     *********************/
static void PrintArray(int array[], int nLength_);
int main(int argc, char* argv[])
{
	int test[10] = {12, 12, 4, 0, 8, 5, 2, 3, 9, 8};
	std::cout << "排序前：" << std::endl;
	PrintArray(test, 10);
	CountingSort(test, 10, 12);
	std::cout << "排序后：" << std::endl;
	PrintArray(test, 10);

	return 0;
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
