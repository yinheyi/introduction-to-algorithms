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
*   Created Time: 2019年06月08日 星期六 10时03分26秒
*   Modifed Time: 2019年06月09日 星期日 23时13分25秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/
typedef unsigned long int size_t;


// 定义一个枚举类型，表示红黑树的颜色
 enum RBColor
{
	RED,
	BLACK,
};

// 红黑树结点的定义
struct RBNode
{
	RBNode();
	int m_nValue;
	int m_nColor;
	RBNode* m_pParent;
	RBNode* m_pLeft;
	RBNode* m_pRight;
};

// 红黑树类的定义
class RBTree
{
public:
	RBTree();
	~RBTree();
	RBNode* search(int nValue_) const;
	bool insert(int nValue_);
	bool erase(RBNode* pNode_);
	size_t size() const;
	bool empty() const;
	int min() const;
	int max() const;
	
protected:
	void transplant(RBNode* pOldNode_, RBNode* pNewNode_);		// 以新树代替旧树
	RBNode* precursor(RBNode* pNode_)const ;		// 求前驱结点
	RBNode* successor(RBNode* pNode_) const;		// 求后驱结点
	RBNode* min_node(RBNode* pNode_) const;			// 查找给定子树中最小结点的指针
	RBNode* max_node(RBNode* pNode_) const;			// 查找给定子树中最大结点的指针
	void left_rotate(RBNode* pNode_);				// 左旋操作
	void right_rotate(RBNode* pNode_);				// 右旋操作
	void insert_fixup(RBNode* pNode_);				// 插入之后调整红黑树，使其符合红黑树的性质
	void erase_fixup(RBNode* pNode_, RBNode* pParent_);	// 删除之后调整红黑树，使其符合红黑树的性质
	RBNode* sibling(RBNode* pNode_) const;			// 获取给定结点的兄弟结点
	void free_memory(RBNode* pNode);				// 释放给定红黑树的内存空间
private:
	size_t m_nSize;
	RBNode* m_pRoot;
};
