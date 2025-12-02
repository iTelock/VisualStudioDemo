// BinaryHeap.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>
#include <vector>
//#include <algorithm>

class BinaryHeap
{
public:
	BinaryHeap(const std::vector<int>& data);
	~BinaryHeap();
	void push(const int& num);	// 插入数据.
	int top();			// 查询数据.
	void pop();			// 删除数据.
	bool empty();		// 查询是否为空.
	int size();			// 查询数组/二叉堆大小.
	void print();		// 打印数据.
private:
	std::vector<int> m_Data;
};



// TODO: 在此处引用程序需要的其他标头。
