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
*   Created Time: 2019年06月16日 星期日 22时21分13秒
*   Modifed Time: 2019年06月16日 星期日 22时39分37秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/
#include "vertex.h"
#include <cassert>
typedef void (*Callback)(vertex*);

// 假设一个无向图是连通的，给定一个图的顶点，进行深度优先搜索, 就可以把所有顶点都遍历完成。
// 为了程序简单，使用递归来完成。
void DepthFirstSearch(vertex* pVertex_, Callback pFunc_)
{
	// 为空或者应该遍历了，返回。
	if (!pVertex_ || pVertex_->m_bSearched)
		return;

	// 遍历当前的顶点
	pFunc_(pVertex_);
	pVertex_->m_bSearched = true;

	// 递归深度优先遍历与它相连的顶点
	for (std::list<vertex*>::iterator _it = pVertex_->m_listAdjacent.begin();
			_it != pVertex_->m_listAdjacent.end(); ++_it)
	{
		if (!(*_it)->m_bSearched)
			DepthFirstSearch(*_it, pFunc_);
	}
}

/**********************    测试程序     *************************/
int main(int argc, char* argv[])
{
	return 0;
}

