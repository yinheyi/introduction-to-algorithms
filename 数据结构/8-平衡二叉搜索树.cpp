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
*   Created Time: 2019年05月18日 星期六 16时38分08秒
*   Modifed Time: 2019年05月27日 星期一 23时35分19秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/
#include <iostream>


// 1.平衡二叉搜索树在二叉搜索树的基础上增加了一个限制条件：要求左右子树的高度差不得大于1.
// 2.平衡二叉搜索树又称作AVL树,AVL是它的两个发明者Adelson-Velskii和Landis名字中的字母。
// 3.平衡二叉搜索树的难点在于当二叉树不满足限制条件时，如何调整树的结构, 这里就涉及到结点
// 的左旋和右旋问题。
// 4.对于平衡二叉搜索树的结点来说，需要在二叉搜索树的基础上增加一个表示左右子树高度差的值，
// 使用平衡因子(balance factor, BF)表示，它表示左子树的高度减右子树的高度。节点的定义如下：
// 5. 在二叉搜索树的基础上，对于插入和删除操作，增加了平衡判断和平衡调整。
//
//
//
/************* 平衡二叉搜索树的节点定义  *************/
struct Node
{
	int m_nValue;
	int m_nBF;		// 对于平衡二叉树来说，平衡因子为-1/0/1。
	Node* m_pParent;
	Node* m_pLeft;
	Node* m_pRight;
	Node(int nValue_ = 0, int nBF_ = 0, 
		Node* pParent_ = nullptr, Node* pLeft_ = nullptr, Node* pRight_ = nullptr);
};

// node的构造函数。
Node::Node(int nValue_, int nBF_, Node* pParent_, Node* pLeft_, Node* pRight_)
{
	m_nValue = nValue_;
	m_nBF = nBF_;
	m_pParent = pParent_;
	m_pLeft = pLeft_;
	m_pRight = pRight_;
}


/********     平衡二叉搜索树类的定义    **********/
class AVLTree
{
public:
	size_t size();
	Node* search(int nValue_);
	void erase(Node* pNode);
	void insert(Node* pNode);
	Node* Precursor();		// 返回前驱节点, 不存在返回空
	Node* Successor();		// 获取后驱节点，不存在返回空

private:
	Node* m_pRoot;
};


/**********************    测试程序     *************************/
int main(int argc, char* argv[])
{
	return 0;
}

