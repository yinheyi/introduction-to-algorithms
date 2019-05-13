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
*   Created Time: 2019年05月13日 星期一 21时06分20秒
*   Modifed Time: 2019年05月13日 星期一 23时05分52秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/
#include <iostream>

// 链表，没有什么好多说的，很常用的数据结构.
// 在单向链表中，非循环的链表的最后一个节点的next指针为空;如果是循环链表，则最后一个节
// 节的next指针指向头指针。
//
// 1.让我们来想一下对于一个单向链表，它需要什么属性信息：它只需要一个pHead来表示链表的头
// 节点的指针就够了！
// 2. 当给定我们一个节点时，如果让我们删除该节点，则需要花费O(n)的时间去找到该节点的前驱
// 节点才能删除该节点，效率很差。所以呢，在c++的stl中实现的单向链表规定：给定一个节点时，
// 我们来删除它后面的那个节点, 即erase_after. 对于插入节点时，也是类似的，即inser_after.
// 3. 现在我们可以会问，我们应该如何删除或者插入头节点呢？？为了与上面的实现一致，我们可
// 以加入一个哨兵节点，来表示为空，记作为pHead。因此给定pHead时，就可以在删除链表的头节点
// 或在链表头部插入一个节点了。
//
//
// 对于单向链表说明强调几点：
// 1. 单向链表适合在给定的节点之后进行插入或删除节点。
// 2. 为了不处理特殊的头节点，使代码很简单, 使用一个哨兵节点来表示为空。
// 3. 当链表为空时，存在一个哨兵节点，它的pNext 为空。
/*******************    单向链表     ********************/
// 单向链表节点的定义
template <typename T>
struct Node
{
	T value;
	Node* pNext = nullptr;
	Node();
	explicit Node(const T& value_, Node<T>* pNext_ = nullptr);
};

template <typename T>
Node<T>::Node()
{
	pNext = nullptr;
}
template <typename T>
Node<T>::Node(const T& value_, Node<T>* pNext_)
{
	value = value_;
	pNext = pNext_;
}

template <typename T>
class forward_list
{
public:
	forward_list();
	~forward_list();
	void insert_after(Node<T>* pCurrent, const T& value);	//插入
	void erase_after(Node<T>* pCurrent);			// 删除
	Node<T>* Head() const;							// 获得链表的头节点
	bool empty() const;								// 判断链表是否为空
	size_t size() const;							// 链表内元素的个数

private:
	Node<T>* m_pHead;
	size_t m_nSize;
};

template <typename T>
forward_list<T>::forward_list()
{
	m_pHead = new Node<T>;
	m_nSize = 0;
}

template <typename T>
forward_list<T>::~forward_list()
{
	while (m_pHead != nullptr)
	{
		Node<T>* _pTemp = m_pHead;
		m_pHead = m_pHead->pNext;
		delete _pTemp;
		_pTemp = nullptr;
	}
}

template <typename T>
void forward_list<T>::insert_after(Node<T>* pCurrent, const T& value)
{
	if (nullptr == pCurrent)
	{
		std::cerr << "指针为空, 插入失败" << std::endl;
	}
	pCurrent->pNext = new Node<T>(value, pCurrent->pNext);
	++m_nSize;
}

template <typename T>
void forward_list<T>::erase_after(Node<T>* pCurrent)
{
	if (nullptr == pCurrent)
	{
		std::cerr << "指针为空, 删除失败" << std::endl;
	}

	Node<T>* _pTemp = pCurrent->pNext;
	pCurrent->pNext = _pTemp->pNext;
	delete _pTemp;
	_pTemp = nullptr;
	--m_nSize;
}

template <typename T>
Node<T>* forward_list<T>::Head() const
{
	return m_pHead;
}

template <typename T>
bool forward_list<T>::empty() const
{
	return 0 == m_nSize;
}

template <typename T>
size_t forward_list<T>::size() const
{
	return m_nSize;
}

/********************    实现单向循环链表类    *************/
// 对于实现单向循环链表类的话，很简单，对上面的类只需要改动一处
// 即可， 即把构造函数修改如下：
//
// 修改前：
template <typename T>
forward_list<T>::forward_list()
{
	m_pHead = new Node<T>;
	m_nSize = 0;
}

// 修改后：
template <typename T>
forward_list<T>::forward_list()
{
	m_pHead = new Node<T>;
	m_pHead->pNext = m_pHead;
	m_nSize = 0;
}

/**********************    测试程序     *************************/
int main(int argc, char* argv[])
{
	forward_list<int> _slist;
	std::cout << "链表是否为空：" << _slist.empty() << std::endl;
	std::cout << "链表内元素个数为：" << _slist.size() << std::endl;
	_slist.insert_after(_slist.Head(), 1);
	_slist.insert_after(_slist.Head(), 2);
	_slist.insert_after(_slist.Head(), 3);
	_slist.insert_after(_slist.Head(), 4);
	std::cout << "链表是否为空：" << _slist.empty() << std::endl;
	std::cout << "链表内元素个数为：" << _slist.size() << std::endl;
	_slist.erase_after(_slist.Head());
	_slist.erase_after(_slist.Head());
	_slist.erase_after(_slist.Head());
	_slist.erase_after(_slist.Head());
	std::cout << "链表是否为空：" << _slist.empty() << std::endl;
	std::cout << "链表内元素个数为：" << _slist.size() << std::endl;

	return 0;
}

