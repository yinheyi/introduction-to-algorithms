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
*   Created Time: 2019年05月12日 星期日 13时44分53秒
*   Modifed Time: 2019年05月12日 星期日 14时49分53秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/
#include <iostream>
#include <stdexcept>

/*************************   栈的简单介绍     ****************************/
// 1. 栈是很常用的数据结构，可能程序员使用栈的机会比数组少，但是，在程序调用过程中一直在
//   使用栈来完成现场的保存与恢复。
// 2. 栈实现的是后进先后的策略，在stl(standard template library)中的栈的实现都是采用适配
// 器的设计模式完成，也就是底层使用vector或list,然后对它们的接口封装成栈的接口。
// 3. 栈通常会有以下几个接口：
//      top(), 返回栈顶的元素
//      push(), 入栈操作
//      pop(), 出栈操作
//      empty(), 判断栈是否为空
//      size(), 栈中元素的个数
// 4.下面基于数组来实现栈的定义， 基于数组来实现的栈是有最大容量的.
//
//
//
/*************************   stack类的定义     ****************************/
// 定义一个栈的模板类
template <typename T>
class Stack
{
public:
	Stack();
	explicit Stack(size_t capicity);
	~Stack();

	void Push(const T& Value);
	void Pop();
	T Top() const;

	bool Empty() const;
	bool Full() const;
	size_t Size() const;
	size_t Capicity() const;

private:
	size_t m_nCapicity;		// 数组最大容量
	size_t m_nSize;			// 当前数组内包含的元素个数
	T* m_pArray;			// 一维数组的指针
};

/*************************   stack类的的实现     ****************************/
// 构造与析构函数的定义
template <typename T>
Stack<T>::Stack()
{
	// 默认的容量大小为1000.
	m_nCapicity = 1000;
	m_nSize = 0;
	m_pArray = new T[m_nCapicity];
}

template <typename T>
Stack<T>::Stack(size_t capicity)
{
	m_nCapicity = capicity;
	m_nSize = 0;
	m_pArray = new T[m_nCapicity];
}

template <typename T>
Stack<T>::~Stack()
{
	delete [] m_pArray;
	m_pArray = nullptr;
}

// 入栈/出栈/获取栈顶元素的函数定义
template <typename T>
void Stack<T>::Push(const T& value)
{
	// 栈否为满的判断
	if (Full())
		std::cerr << "栈为满，入栈失败" << std::endl;
	else
		m_pArray[m_nSize++] = value;
}

template <typename T>
void Stack<T>::Pop()
{
	// 栈否为空的判断
	if (Empty())
		std::cerr << "栈为空，出栈失败" << std::endl;
	else
		--m_nSize;
}

template <typename T>
T Stack<T>::Top() const
{
	if (Empty())
		throw std::out_of_range("栈空，不存在栈顶元素");
	else
		return m_pArray[m_nSize - 1];
}

// 栈空/栈满/栈内元素个数的相关函数
template <typename T>
bool Stack<T>::Empty() const
{
	return 0 == m_nSize;
}

template <typename T>
bool Stack<T>::Full() const
{
	return m_nSize == m_nCapicity;
}

template <typename T>
size_t Stack<T>::Size() const
{
	return m_nSize;
}

template <typename T>
size_t Stack<T>::Capicity() const
{
	return m_nCapicity;
}

/***************    测试程序   ****************************/
int main(int argc, char* argv[])
{
	Stack<int> stack(10);
	std::cout << "栈的容量为：" << stack.Capicity() << std::endl;
	std::cout << "栈内元素个数为：" << stack.Size() << std::endl;

	stack.Push(10);
	stack.Push(100);
	stack.Push(1000);
	std::cout << "栈内元素个数为：" << stack.Size() << std::endl;
	std::cout << "栈顶的元素为：" << stack.Top() << std::endl;

	stack.Pop();
	std::cout << "栈顶的元素为：" << stack.Top() << std::endl;

	stack.Pop();
	std::cout << "栈顶的元素为：" << stack.Top() << std::endl;
	stack.Pop();

	stack.Pop();
	stack.Top();

	return 0;
}

