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
*   Created Time: 2019年05月07日 星期二 21时41分02秒
*   Modifed Time: 2019年05月08日 星期三 00时36分38秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/


// 堆使用一个数组表示， 它可以当作一个近似的完全二叉树，除了最底层之外，其它层都是满的，
// 并且最底层也是从左到右填充的。
//
// 当堆的下标（数组的下标）从1开始时比较好计算。因为：
// 		1. 当父结点为i时， 左孩子为2i,  右孩子为2i+1;
// 		2. 当孩子结点的下标为j时，父结点的下标为j/2	(根结点为父结点）;
// 		3. 一个结点的下标为k时， 则它所有的深度为 floor(logK);
// 		4. 当一个堆共n个元素时，它的高度为floor(logN).
//
//                    1
//                /       \
//             2             3
//           /   \         /    \
//         4      5       6       7
//        / \    / \     /  \   /   \
//      8    9  10  11  12   13 14   15
//
// 
// 但是呢，数组的下标都是从0开始的，所以呢，我们下代码时，还是需要从0开始,而此时：
//     1. 当父结点的下标为i时，左孩子为2i+1, 右孩子为2*(i+1)
//     2. 当孩子结点的下标为j时，父结点的下标为（j-1)/2.
//
//                    0
//                /       \
//             1             2
//           /   \         /    \
//         3      4       5       6
//        / \    / \     /  \   /   \
//      7    8  9  10  11   12 13   14
//
//
//
/**************************     代码如下      ****************************/

// 定义三个宏，分别用于求左孩子/右孩子/父结点的下标。
#define LEFT(i) (((i) << 1) + 1)
#define RIGHT(i) (((i) + 1) << 1)
#define PARENT(i) (((i) - 1) >> 1)

// 大于比较函数
bool less(int lhs, int rhs)
{
	return lhs < rhs;
}

// 大于比较函数
bool greate(int lhs, int rhs)
{
	return lhs > rhs;
}
typedef bool (*Comp)(int, int);

// 交换两个元素的值
static inline void swap(int& lhs, int & rhs)
{
	int _nTemp = lhs;
	lhs = rhs;
	rhs = _nTemp;
}

// 假设一个节点的左子树与右子树都满足堆的性质，而该节点不满足最大堆或最小堆的性质，该
// 函数实现调整节点位置，维护堆的性质。
// 输入参数分别表示: 堆的数组/数组长度/节点i的下标/表示比较的二元谓词
// 复杂度为O(logN).
void Heapify(int array[], int nLength_, int nIndex_, Comp CompFunc)
{
	if (array == nullptr || nIndex_ >= nLength_ || nIndex_ < 0 || CompFunc == nullptr)
		return;

	int _nLeft = LEFT(nIndex_);
	int _nRight = RIGHT(nIndex_); 

	// 初始化最大值节点的下标;
	int _nLargest = nIndex_;
	if ( _nLeft < nLength_ && !CompFunc(array[_nLargest], array[_nLeft]))
	{
		_nLargest = _nLeft;
	}
	if (_nRight < nLength_ && !CompFunc(array[_nLargest], array[_nRight]))
	{
		_nLargest = _nRight;
	}

	/* 此时不需要维护堆的性质，直接返回   */
	if (_nLargest == nIndex_)
	{
		return;
	}

	swap(array[nIndex_], array[_nLargest]);
	Heapify(array, nLength_, _nLargest, CompFunc);
}

// 使用一个数组建立一个最小堆或最大堆。
// 输入参数为：一维数组/数组的长度/表示比较的二元谓词，用于控制建最小堆还是最大堆
// 复杂度为O(N).
void BulidHeap(int array[], int nLength_, Comp CompFunc)
{
	if (array == nullptr || nLength_ <= 1 || CompFunc == nullptr)
		return;

	// 从最后一个元素的父节点开始调用Heapify()函数来建堆。
	for (int i = PARENT(nLength_ - 1); i >=0; --i)
	{
		Heapify(array, nLength_, i, CompFunc);
	}
}

// 堆排序的函数
// 说明：1. 通过建立[最大堆]可以实现[从小到大]的排序;
//       2. 通过建立[最小堆]可以实现[从大到小]的排序
//       3. 堆排序为原址排序，它不需要借助额外的空间.(归并排序不是原址排序）
//       4. 堆排序的复杂度为O(NlogN).
//
// 堆排序的思想 (以从小到大排序说明）：
//       第一步：建立一个最大堆;
//       第二步：把首元素与最后一个元素进行交换;
//       第三步：把堆的大小减1,对新的堆进行维护维的性质;
//       第四步：把首元素与倒数第二个元素进行交换;
//       第五步：把堆的大小减1,对新的堆进行维护维的性质;
//       .......
//
void HeapSort(int array[], int nLength_, Comp CompFunc)
{
	if (array == nullptr || nLength_ <=1 || CompFunc == nullptr) 
		return;

	BulidHeap(array, nLength_, CompFunc);
	for (int i = nLength_; i >= 2; --i)
	{
		Heapify(array, i, 0, CompFunc);
		swap(array[0], array[i - 1]);
	}
}


/************    测试     *****************/
#include <iostream>

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

// 主函数
int main(int argc, char* argv[])
{
	int array[10] = { 100, 1, 1, -1243, 0, 223, 443, 123, -12, -129};

	PrintArray(array, 10);
	HeapSort(array, 10, greate);
	PrintArray(array, 10);

	return 0;
}
