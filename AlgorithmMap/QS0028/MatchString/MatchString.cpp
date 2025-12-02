// MatchString.cpp: 定义应用程序的入口点。
//

#include "MatchString.h"

/*
 * 题目：实现 strStr() —— 返回 needle 在 haystack 中第一次出现的起始下标，若不存在返回 -1。
 *
 * 方法：Rabin–Karp（多项式滚动哈希）+ 二次比对
 * 哈希形式（模 2^64 的隐式取模）：H(s[0..m-1]) = s[0]*B^(m-1) + s[1]*B^(m-2) + ... + s[m-1]
 *
 * 关键点：
 * 1) 使用 unsigned long long（64 位无符号整数）。在 C++ 中，其溢出是“定义良好”的：自动按 2^64 回绕，
 *    等价于对 2^64 取模（不需要写 % 运算）。这是“隐式模数”。
 * 2) B（基数）选为奇数（如 131）。模数是 2^64（2 的幂），奇数基数更能保持低位信息，不易退化。
 * 3) 计算时把字符强转为 unsigned char，避免 char 为有符号实现时的负值参与运算导致哈希异常。
 * 4) 为了 O(1) 地滑动窗口，要预先计算 powB = B^(m-1)（同样在模 2^64 的意义下），用于“去掉最左字符的权重”。
 * 5) 哈希相等后，务必做一次 O(m) 的字符串逐字比对（compare），消除低概率哈希碰撞的影响。
 *
 * 变量含义：
 *   n, m      : 主串/模式串长度
 *   B         : 基数（常用 131/137/257 等）
 *   hp        : 模式串 needle 的哈希
 *   hw        : 主串 haystack 当前窗口（长度 m）的哈希
 *   powB      : B^(m-1)（去头时使用）
 *
 * 复杂度：
 *   时间：期望 O(n)。每次滑动 O(1)，极少数碰撞时做 O(m) 的二次比对。
 *   空间：O(1)。
 */

int main()
{
	std::cout << "QS0028 找出字符串中第一个匹配项的下标." << std::endl;
	std::string haystack = "abcdefg";
	std::string needle = "bcd";
	Solution s;
	int result = s.strStr(haystack, needle);
	std::cout << "匹配位置: " << result << std::endl;
	return 0;
}

// 方法1：Rabin–Karp（多项式滚动哈希）+ 二次比对.
int Solution::strStr(std::string haystack, std::string needle)
{
	using ull = unsigned long long;
	int sizeT = static_cast<int>(haystack.size());		// 匹配串的长度.
	int sizeP = static_cast<int>(needle.size());		// 模式串的长度.
	int base = 131;	// 基数.

	// 1. 预处理.
	ull hashP = 0;	// 模式串的哈希.
	ull hashW = 0;	// 窗口的哈希.
	ull powB = 1;	// B^(m-1), 窗口首位的基数位值.
	
	for (int i = 0; i < sizeP; i++) {
		// 1.1 计算模式串的哈希.
		hashP = hashP * base + static_cast<unsigned char>(needle[i]);	// 不同平台char实现不一样，signed -128 ~ 127或unsigned 0 ~ 255.
		// 1.2 计算首个窗口的哈希.
		hashW = hashW * base + static_cast<unsigned char>(haystack[i]);
		// 1.3 计算窗口“去头”的基数位值（B^m-1）.
		if (i < sizeP - 1) {
			powB *= base;
		}
	}
	// 1.4 初始位置验证.
	if (hashP == hashW && haystack.compare(0, sizeP, needle) == 0) {
		return 0;	// 首个窗口即可匹配.
	}

	// 2. 窗口滑动匹配.
	for (int i = sizeP; i < sizeT; i++) {	// i 为进入窗口新字符的下标.
		// 2.1 去头：去除窗口首项哈希值.
		hashW -= static_cast<ull>(static_cast<unsigned char>(haystack[i - sizeP])) * powB;
		// 2.2 左移：剩余每位升高位次.
		hashW *= base;
		// 2.3 加尾：加上移入窗口的新元素.
		hashW += static_cast<ull>(static_cast<unsigned char>(haystack[i]));
		// 2.4 根据匹配结果返回值.
		int start = i - sizeP + 1;
		if (hashW == hashP && haystack.compare(start, sizeP, needle) == 0) {
			return start;
		}
	}

	return -1;
}

