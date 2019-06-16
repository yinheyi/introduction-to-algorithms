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
*   Created Time: 2019年06月16日 星期日 21时43分38秒
*   Modifed Time: 2019年06月16日 星期日 22时11分24秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/
#include "vertex.h"
#include <cassert>
#include <queue>
typedef void (*Callback)(vertex*);

// 给定一个图的顶点，进行广度优先搜索
void BreadthFirstSearch(vertex* pVertex_, Callback pFunc_)
{
	if (!pVertex_)
		return;

	std::queue<vertex*> _queue;
	_queue.push(pVertex_);

	while (!_queue.empty())
	{
		// 获取队列元素, 并使用回调函数进行处理。
		vertex* _pCurrent = _queue.front();
		pFunc_(_pCurrent);
		_pCurrent->m_bSearched = true;
		_queue.pop();

		// 把与该顶点相连的顶点添加至队列中。
		for (std::list<vertex*>::iterator _it = _pCurrent->m_listAdjacent.begin();
				_it != _pCurrent->m_listAdjacent.end(); ++_it)
		{
			// 如果没有被遍历，则加入到队列中。
			if (!(*_it)->m_bSearched)
				_queue.push(*_it);
		}
	}
}


/**********************    测试程序     *************************/
int main(int argc, char* argv[])
{
	return 0;
}

