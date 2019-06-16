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
*   Created Time: 2019年06月16日 星期日 21时29分16秒
*   Modifed Time: 2019年06月16日 星期日 22时11分26秒
*   Blog: http://www.cnblogs.com/yinheyi
*   Github: https://github.com/yinheyi
*   
***********************************************************************/
#include <list>

// 图中顶点结构体的定义
struct vertex
{
	int m_nValue;				// 顶点本身的值
	bool m_bSearched;			// 该结点是否被搜索过
	std::list<vertex*> m_listAdjacent;		// 与该顶点相连的邻居顶点
};
