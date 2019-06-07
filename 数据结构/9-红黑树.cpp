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
*   Created Time: 2019年05月18日 星期六 16时38分00秒
*   Modifed Time: 2019年05月18日 星期六 16时38分04秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/
#include <iostream>


// 定义一个枚举类型，表示红黑树的颜色
 enum RBColor
{
	RED,
	BLACK,
}

// 红黑树结点的定义
struct RBNode
{
	RBNode();
	int m_nValue;
	int m_nColor;
	RBNode* m_pParent;
	RBNode* m_pLeft;
	RBNode* m_pRigh;
};

// 红黑树结点的构造函数
RBNode::RBNode()
{
	m_nValue = 0;
	m_nColor = RED;
	m_pParent = nullptr;
	m_npLeft = nullptr;
	m_npRight = nullptr;
}

// 红黑树类的定义
class RBTree
{
public:
	RBTree();
	~RBTree();
	bool insert(int nValue_);
	bool erase(RBNode* pNode_);
	size_t size();
	bool empty();
	int min() const;
	int max() const;
	
protected:
	void Transplant(RBNode* pOldNode_, RBNode* pNewNode_);		// 以新树代替旧树
	RBNode* precursor(RBNode* pNode_);	// 求前驱结点
	RBNode* successor(RBNode* pNode_);	// 求后驱结点
	RBNode* min_node(RBNode* pNode_);	// 查找给定子树中最小结点的指针
	RBNode* max_node(RBNode* pNode_);	// 查找给定子树中最大结点的指针
	void free_memory(RBNode* pNode);	// 释放给定红黑树的内存空间
private:
	size_t m_nSize;
	RBNode* m_pRoot;
};

// 
}

/**********************    测试程序     *************************/
int main(int argc, char* argv[])
{
	return 0;
}

