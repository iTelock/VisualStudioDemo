// KMP.cpp: 定义应用程序的入口点。
//

#include "KMP.h"

int main()
{
	std::cout << "QS686. 重复叠加字符串匹配." << std::endl;
	//std::string a = "abcd";
	//std::string b = "bc";
	// 
	//std::string a = "abcd";
	//std::string b = "cdabcdab";

	std::string a = "a";
	std::string b = "aa";

	//std::string a = "bacbabababababaca";
	//std::string b = "ababaca";

	//std::string a = "abaababa";
	//std::string b = "abaabaa";
	Solution s;
	int result = s.repeatedStringMatch(a, b);
	std::cout << "匹配次数为：" << result << std::endl;
	return 0;
}

int Solution::repeatedStringMatch(std::string a, std::string b)
{
	// 1. a 的复制次数讨论.
	int aSize = a.size();
	int bSize = b.size();
	int times = (aSize + bSize - 1) / aSize;
	std::string text1 = a;
	std::string text2 = a;
	for (int i = 1; i < times; i++) {
		text1 += a;
	}
	text2 = text1 + a;

	// 2. KMP匹配.
	if (kmpFunc(text1, b)) {
		return times;
	}
	else if (kmpFunc(text2, b)) {
		return times + 1;
	}

	return -1;
}

// KMP算法入口，返回true匹配，false不匹配.
bool Solution::kmpFunc(const std::string& text, const std::string& pattern)
{
	std::vector<int> lps(pattern.size(), 0);
	kmpBuild(lps, pattern);

	std::cout << "LPS数组：";
	for (auto i : lps) {
		std::cout << i << ", ";
	}
	std::cout << std::endl;

	int index = kmpMatch(lps, text, pattern);

	if (index < 0) {
		std::cout << "匹配失败" << std::endl;
		return false;
	}
	std::cout << "匹配成功, 起始位置：" << index << std::endl;
	return true;
}

// 构建KMP的LPS数组.
void Solution::kmpBuild(std::vector<int>& lps, const std::string& pattern)
{
	int sizeP = pattern.size();
	int len = 0;
	int i = 1;
	while(i < sizeP) {
		if (pattern[len] == pattern[i]) {
			len++;
			lps[i] = len;
			i++;
		}
		else if (0 != len) {
			len = lps[len - 1];
		}
		else {
			lps[i] = 0;
			i++;
		}
	}
}

// KMP匹配阶段(返回匹配的text下标位置，不匹配返回-1).
int Solution::kmpMatch(const std::vector<int>& lps, const std::string& text, const std::string& pattern)
{
	int sizeT = text.size();
	int sizeP = pattern.size();
	int i = 0;
	int j = 0;
	int index = 0;
	while ((i < sizeT) && (j < sizeP)) {
		if (pattern[j] == text[i]) {
			i++;
			j++;
		}
		else if (0 == j) {
			i++;
		}
		else {
			j = lps[j - 1];
		}

		if (sizeP == j) {
			return i - j;
		}
	}
	return -1;
}
