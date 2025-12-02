// BinaryHeap.cpp: 定义应用程序的入口点。
//

#include "BinaryHeap.h"

int main()
{
	std::vector<int> data = { 3,2,1,5,3 };
	int pushData = 8;
	BinaryHeap bh(data);
	bh.print();
	bh.push(pushData);
	bh.print();
	bh.pop();
	bh.print();
	return 0;
}

BinaryHeap::BinaryHeap(const std::vector<int>& data)
{
	if (data.size() == 0) {
		std::cout << "初始化数据为空!" << std::endl;
		m_Data = {};
		return;
	}
	for (auto i : data) {
		push(i);
	}
}

BinaryHeap::~BinaryHeap()
{
}

void BinaryHeap::push(const int& num)
{
	int index = m_Data.size();
	m_Data.push_back(num);
	while (index > 0) {
		int parrent = (index - 1) / 2;
		if (m_Data[index] <= m_Data[parrent]) {
			break;	// 满足条件，无需继续上浮.
		}
		std::swap(m_Data[index], m_Data[parrent]);	// 上浮.
		index = parrent;	// 更新index，继续上浮.
	}
}

int BinaryHeap::top()
{
	if (empty()) {
		throw std::out_of_range("二叉堆为空！");
	}
	return m_Data[0];
}

void BinaryHeap::pop()
{
	if (empty()) {
		throw std::out_of_range("二叉堆为空！");
	}

	m_Data[0] = m_Data.back();
	m_Data.pop_back();
	// 下沉操作.
	int index = 0;
	int size = m_Data.size();
	while (true) {
		int leftChild = index * 2 + 1;
		int rightChild = index * 2 + 2;
		int maxIndex = index;
		if (leftChild < size && m_Data[leftChild] > m_Data[maxIndex]) {
			maxIndex = leftChild;
		}
		if (rightChild < size && m_Data[rightChild] > m_Data[maxIndex]) {
			maxIndex = rightChild;
		}
		if (maxIndex == index) {
			break;
		}
		std::swap(m_Data[index], m_Data[maxIndex]);
		index = maxIndex;	// 指向交换后的位置，继续下沉.
	}
}

bool BinaryHeap::empty()
{
	return m_Data.empty();
}

int BinaryHeap::size()
{
	return m_Data.size();
}

void BinaryHeap::print()
{
	for (auto i : m_Data) {
		std::cout << i << ", ";
	}
	std::cout << std::endl;
}


