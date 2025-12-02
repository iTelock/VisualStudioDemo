#ifndef BASICHUFFMAN_H
#define BASICHUFFMAN_H

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

class BasicHuffman
{
public:
	BasicHuffman() = default;
	~BasicHuffman() = default;
	void start();
};

class HuffmanNode
{
public:
	HuffmanNode();
	~HuffmanNode() = default;

private:
	char data_;	// ±àÂë×Ö·û.
	int freq_;		// ³öÏÖÆµÂÊ.
	std::unique_ptr<HuffmanNode> left_child_;
	std::unique_ptr<HuffmanNode> right_child_;
};

HuffmanNode::HuffmanNode()
{
}







#endif // !BASICHUFFMAN_H
