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
*   Created Time: 2019年05月12日 星期日 14时54分33秒
*   Modifed Time: 2019年05月12日 星期日 19时38分53秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/
#include <iostream>
#include <stdexcept>

/*******************    队列的简单介绍    ************************/
// 1. 队列是实现一种先进先出的策略，本来队列就是在一端进一端出的数据对构，即单端队列。在
// stl的模板库中加入了两端队列，即两端都可以进也可以出。在stl中单端队列为queue,而双端队列
// 为deque.
// 2. 在stl中单端队列的实现与栈相同，也是通过合适器设计模式来完成的，单端队列的底层是基于
// 双端队列来完成的，而双端队列是基于类似于链表与vector的数据结构结合起来实现的（具体
// 可以参考《stl源码剖析》一书）。
// 3. 在单端队列中，常用的接口为：
//     front()				获取队首的元素
//     enqueue()			入队
//     dequeue()			出队
//     size()				队列中元素的个数
//     empty()				队列是否为空
//
//   在双端队列吕，常用的接口为：
//    front()				获取队首元素
//    back()				获取队尾元素
//    push_back()			在队列尾部插入
//    push_front()			在队列首部插入
//    pop_back()			在队列尾部出队
//    pop_front()			在队列首部出队
//    size()				队列的元素的个数
//    empty()				队列是否为空
//
// 4. 使用数组实现队列时， 有一个问题需要解决，那就是如何去区分队列为空还是满. 假设head
// 指向第一个非空位置的下标，tail指向一个为空位置的下标, 当队列为空或队列为满时，head与
// tail指向的位置都是重合的。有两种解决方案来解决此问题：
//    方案一：当使用容量为N的数组来实现队列时，最多只存放N-1个元素, 则此时：
//        当队列为空时，有head == tail
//        当队列为满时，有(tai+1)%N == head
//    方案二：增加一个bool变量m_bEmpty()用于标记队列为空的状态, 此时：
//        当队列为空时，有head == tail && m_bEmpty==true;
//        当队列为满时，有head == tail && m_bEmpty!=true
// 我个人更倾向于方案一，简洁，不需要添加多余的变量. 下面队列的实现采用方案一。
//
/*******************    基于数组来实现单端队列        *****************/
template <typename T>
class queue
{
public:
	queue();
	queue(size_t nCapicity);
	~queue();
	void enqueue(const T& value);		// 入队
	void dequeue();						// 出队
	T front() const;					// 获取队首元素
	size_t size() const;				// 队内元素的个数
	size_t capicity() const;			// 队容量大小
	bool empty() const;					// 队是否为满
	bool full() const;					// 队是否为空

private:
	size_t m_nArrayLength;				// 底层数组的长度
	size_t m_nHead;						// 队首元素的下标
	size_t m_nTail;						// 队尾元素的下一个位置的下标，即指向一个为空的位置
	T* m_pArray;
};

template <typename T>
queue<T>::queue()
{
	m_nArrayLength = 1001;
	m_nHead = 0;
	m_nTail = 0;
	m_pArray = new T[m_nArrayLength];
}

template <typename T>
queue<T>::queue(size_t capicity)
{
	m_nArrayLength = capicity + 1;
	m_nHead = 0;
	m_nTail = 0;
	m_pArray = new T[m_nArrayLength];
}

template <typename T>
queue<T>::~queue()
{
	m_pArray = nullptr;
}

template <typename T>
void queue<T>::enqueue(const T& value)
{
	// 队列为满判断
	if ((m_nTail + 1) % m_nArrayLength == m_nHead)
	{
		std::cerr << "队列为满，入队失败" << std::endl;
		return;
	}

	m_pArray[m_nTail] = value;
	m_nTail = (m_nTail + 1) % m_nArrayLength;
}

template <typename T>
void queue<T>::dequeue()
{
	// 队列的空判断
	if (m_nHead == m_nTail)
	{
		std::cerr << "队列为空，出队失败" << std::endl;
		return;
	}

	m_nHead = (m_nHead + 1) % m_nArrayLength;
}

template <typename T>
T queue<T>::front() const
{
	// 队列为空判断
	if (m_nHead == m_nTail)
	{
		throw std::out_of_range("队列为空，不存在队首的元素。");
	}

	return m_pArray[m_nHead];
}

template <typename T>
size_t queue<T>::size() const
{
	if (m_nHead <= m_nTail)
		return m_nTail - m_nHead;
	else
		return m_nTail + m_nArrayLength - m_nHead;
}

template <typename T>
size_t queue<T>::capicity() const
{
	return m_nArrayLength - 1;
}

template <typename T>
bool queue<T>::empty() const
{
	return m_nHead == m_nTail;
}

template <typename T>
bool queue<T>::full() const
{
	return (m_nTail + 1) % m_nArrayLength == m_nHead;
}

/*******************    基于数组来实现双端队列        *****************/
template <typename T>
class deque
{
public:
	deque();
	deque(size_t nCapicity);
	~deque();
	void push_front(const T& value);	// 队首入队
	void pop_front();		  	  		// 队首出队
	T front() const;		  	  		// 队首元素
	void push_back(const T& value);		// 队尾入队
	void pop_back();		  	  		// 队尾出队
	T back() const;			  	  		// 队尾元素
	size_t size() const;	 		  	// 队内元素的个数
	size_t capicity() const;			// 队容量大小
	bool empty()const;					// 队是否为满
	bool full() const;					// 队是否为空
                                                                                         
private:                                                                                 
	size_t m_nArrayLength;          // 底层数组的长度
	size_t m_nHead;                 // 队首元素的下标
	size_t m_nTail;                 // 队尾元素的下一个位置的下标，即指向一个为空的位置
	T* m_pArray;
};

template <typename T>
deque<T>::deque()
{
	m_nArrayLength = 1001;
	m_nHead = 0;
	m_nTail = 0;
	m_pArray = new T[m_nArrayLength];
}

template <typename T>
deque<T>::deque(size_t capicity)
{
	m_nArrayLength = capicity + 1;
	m_nHead = 0;
	m_nTail = 0;
	m_pArray = new T[m_nArrayLength];
}

template <typename T>
deque<T>::~deque()
{
	m_pArray = nullptr;
}

template <typename T>
void deque<T>::push_front(const T& value)
{
	// 队列为满判断
	if ((m_nTail + 1) % m_nArrayLength == m_nHead)
	{
		std::cerr << "队列为满，本次在队首入队失败" << std::endl;
		return;
	}

	m_nHead = (m_nHead  + m_nArrayLength - 1) % m_nArrayLength;
	m_pArray[m_nHead] = value;
}

template <typename T>
void deque<T>::pop_front()
{
	// 队列为空判断
	if (m_nHead == m_nTail)
	{
		std::cerr << "队列为空，本次在队首出队失败。" << std::endl;
		return;
	}

	m_nHead = (m_nHead + 1) % m_nArrayLength;
}

template <typename T>
T deque<T>::front() const
{
	// 队列为空时，抛出异常
	if (m_nHead == m_nTail)
	{
		throw std::out_of_range("队列为空，不存在队首元素");
	}

	return m_pArray[m_nHead];
}

template <typename T>
void deque<T>::push_back(const T& value)
{
	// 队列为满判断
	if ((m_nTail + 1) % m_nArrayLength == m_nHead)
	{
		std::cerr << "队列为满，本次在队尾入队失败" << std::endl;
		return;
	}

	m_pArray[m_nTail] = value;
	m_nTail = (m_nTail + 1) % m_nArrayLength;
}

template <typename T>
void deque<T>::pop_back()
{
	// 队列为空判断
	if (m_nHead == m_nTail)
	{
		std::cerr << "队列为空，本次在队尾出队失败。" << std::endl;
		return;
	}

	m_nTail = (m_nTail + m_nArrayLength - 1) % m_nArrayLength;
}

template <typename T>
T deque<T>::back() const
{
	// 队列为空时，抛出异常
	if (m_nHead == m_nTail)
	{
		throw std::out_of_range("队列为空，不存在队尾元素");
	}

	size_t _nIndex = (m_nTail + m_nArrayLength - 1) % m_nArrayLength;
	return m_pArray[_nIndex];
}

template <typename T>
size_t deque<T>::size() const
{
	if (m_nHead <= m_nTail)
		return m_nTail - m_nHead;
	else
		return m_nTail + m_nArrayLength - m_nHead;
}

template <typename T>
size_t deque<T>::capicity() const
{
	return m_nArrayLength - 1;
}

template <typename T>
bool deque<T>::empty() const
{
	return m_nHead == m_nTail;
}

template <typename T>
bool deque<T>::full() const
{
	return (m_nTail + 1) % m_nArrayLength == m_nHead;
}

/**********************    测试程序     *************************/
int main(int argc, char* argv[])
{
	// 单端队列的测试
	std::cout << "单端队列的测试输出结果："<< std::endl;
	queue<int> _queue(5);
	std::cout << "队列的容量为：" << _queue.capicity() << std::endl;
	std::cout << "队列内元素的个数为：" << _queue.size() << std::endl;
	_queue.enqueue(1);
	_queue.enqueue(2);
	_queue.enqueue(3);
	_queue.enqueue(4);
	_queue.enqueue(5);
	_queue.enqueue(6);
	std::cout << "队列内元素的个数为：" << _queue.size() << std::endl;
	std::cout << "队首的元素为：" << _queue.front() << std::endl;
	_queue.dequeue();
	_queue.dequeue();
	_queue.dequeue();
	_queue.dequeue();
	_queue.dequeue();
	_queue.dequeue();
	// 对空队列执行获取队首元素操作
	try
	{
		_queue.front();
	}
	catch(std::out_of_range& _error)
	{
		std::cout << _error.what() << std::endl;
	}

	// 双端队列的测试
	std::cout << std::endl;
	std::cout << "双端队列的测试输出结果："<< std::endl;
	deque<int> _deque(6);
	std::cout << "双端队列的容量为：" << _deque.capicity() << std::endl;
	std::cout << "双端队列内元素的个数为：" << _deque.size() << std::endl;
	_deque.push_back(3);
	_deque.push_back(4);
	_deque.push_back(5);
	_deque.push_front(2);
	_deque.push_front(1);
	_deque.push_front(0);
	std::cout << "双端队列队首的元素为：" << _deque.front() << std::endl;
	std::cout << "双端队列队尾的元素为：" << _deque.back() << std::endl;

	std::cout << "下面应该提示队列为满，无法入队的信息：" << std::endl;
	_deque.push_back(6);
	_deque.push_front(0);

	_deque.pop_front();
	_deque.pop_front();
	_deque.pop_front();
	_deque.pop_back();
	_deque.pop_back();
	_deque.pop_back();
	std::cout << "下面应该提示队列为空，无法队的信息：" << std::endl;
	_deque.pop_front();
	_deque.pop_back();

	std::cout << "下面应该提示队列为空，不存在队首或队尾的元素：" << std::endl;
	try
	{
		_deque.front();
	}
	catch (std::out_of_range& _error)
	{
		std::cout << _error.what() << std::endl;
	}

	try
	{
		_deque.back();
	}
	catch (std::out_of_range& _error)
	{
		std::cout << _error.what() << std::endl;
	}


	return 0;
}
