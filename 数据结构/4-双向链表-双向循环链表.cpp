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
*   Created Time: 2019年05月13日 星期一 23时20分45秒
*   Modifed Time: 2019年05月14日 星期二 23时04分36秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/
#include <iostream>


// 1. 双向链表与单向链表相比， 增加了一个指向上一个上一个节点的指针，除此之外没有什么特别了。
// 有了这个指向前驱节点的指针，我们这样就可以很方便地通过当前节点找到上一个节点。因此，对
// 于双向链表，我们可以很方便地进行：
//     1. 在O(1)时间内删除给定的结点;
//     2. 在O(1)时间内在给定的结点之前插入一个节点。
//     额外说明：在单向链表中，我们只能在O（1）时间内删除给定结点的下一个结点和在给定结点
//     之后插入一个结点。
// 
// 2.双向链表删除与插入也没有什么其它操作了。在大多数的教科书中，在讲解删除与插入操作时，都
// 会分成三种情况：在链表首部/在链表中间/和链表尾部 进行插入与删除操作。为了不这么麻烦，我
// 们可以增加一个哨兵结点，很代码很简洁，不需要区分三种情况了。
//
//
// 双向链表节点的定义如下(模板类）：
template <typename T>
struct Node
{
	T value;
	Node<T>* pPre;
	Node<T>* pNext;

	// 构造函数
	Node();
	explicit Node(const T& value_, Node<T>* pPre_ = nullptr, Node<T>* pNext_ = nullptr);
};

template <typename T>
Node<T>::Node()
{
	pPre = nullptr;
	pNext = nullptr;
}

template <typename T>
Node<T>::Node(const T& value_, Node<T>* pPre_, Node<T>* pNext_)
{
	value = value_;
	pPre = pPre_;
	pNext = pNext_;
}

/**********************    双向链表模版类的定义与实现      ***************/
template <typename T>
class list
{
public:
	list();		//构造函数
	~list();	//析构函数
	void insert(Node<T>* pCurrent_, const T& value_);		// 在给定结点之前插入一个节点
	void erase(Node<T>* pCurrent_);							// 删除给定的结点值
	Node<T>* Head() const;									// 返回链表头指针
	bool empty() const;
	size_t size() const;

private:
	Node<T>* m_pHead;
	size_t m_nSize;
};

template <typename T>
list<T>::list()
{
	// new出来一个哨兵节点
	m_pHead = new Node<T>;
	m_nSize = 0;
}

template <typename T>
list<T>::~list()
{
	while (m_pHead != nullptr)
	{
		Node<T>* _pNext = m_pHead->pNext;
		delete m_pHead;
		m_pHead = _pNext;
	}
}

// 在给定结点之前插入一个节点
template <typename T>
void list<T>::insert(Node<T>* pCurrent_, const T& value_)
{
	if (nullptr == pCurrent_)
	{
		std::cerr << "给定的指针参数为空，插入失败" << std::endl;
		return;
	}

	// 新new并初始化一个结点
	Node<T>* _pNew = new Node<T>(value_, pCurrent_->pPre, pCurrent_);
	// 更新前驱节点的pNext的值和后驱节点的pPre的值
	pCurrent_->pPre->pNext = _pNew;
	pCurrent_->pPre = _pNew;

	++m_nSize;
}

template <typename T>
void list<T>::erase(Node<T>* pCurrent_)
{
	if (nullptr == pCurrent_)
	{
		std::cerr << "给定的指针参数为空，插入失败" << std::endl;
		return;
	}

	// 更新前驱节点的pNext的值和后驱节点的pPre的值
	pCurrent_->pPre->pNext = pCurrent_->pNext;
	pCurrent_->pNext->pPre = pCurrent_->pPre;
	// delete掉当前节点
	delete pCurrent_;
	pCurrent_ = nullptr;

	--m_nSize;
}

template <typename T>
bool list<T>::empty() const
{
	return 0 == m_nSize;
}

template <typename T>
size_t list<T>::size() const
{
	return m_nSize;
}


/************    双向链表的循环链表的实现    ***************/
// 由双向链表实现循环链表，只需要修改一处代码即可，即：
// 把list的构造函数由修改前的：
template <typename T>
list<T>::list()
{
	m_pHead = new Node<T>;
	m_nSize = 0;
}

// 修改为：
template <typename T>
list<T>::list()
{
	m_pHead = new Node<T>;
	m_pHead->pPre = m_pHead;
	m_pHead->pNext = m_pHead;
	m_nSize = 0;
}

/**********************    测试程序     *************************/
int main(int argc, char* argv[])
{
	return 0;
}

