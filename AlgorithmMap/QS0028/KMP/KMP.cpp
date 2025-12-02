// KMP.cpp: 定义应用程序的入口点。
//

#include "KMP.h"

int main()
{
	std::cout << "QS0028 找出字符串中第一个匹配项的下标." << std::endl;
	std::string haystack = "bacbabababababaca";
	std::string needle = "ababaca";
	Solution s;
	int result = s.strStr(haystack, needle);
	std::cout << "匹配位置: " << result << std::endl;
	return 0;
}

int Solution::strStr(std::string haystack, std::string needle)
{
	int sizeP = static_cast<int>(needle.size());
	auto lps = std::vector<int>(sizeP, 0);
	// 构造LPS数组.
	buildLPS(lps, needle);
	std::cout << "LPS数组：" << std::endl;
	for (auto i : lps) {
		std::cout << i << ", ";
	}
	std::cout << std::endl;
	// 匹配.
	return compare(lps, needle, haystack );
}

void Solution::buildLPS(std::vector<int>& lps, const std::string& pattern)
{
	int sizeP = static_cast<int>(pattern.size());
	int length = 0;
	for (int i = 1; i < sizeP; i++) {
		if (pattern[length] == pattern[i]) {
			length++;
			lps[i] = length;
		}
		else {
			if (length > 0) {
				length = lps[length - 1];
				i--;	// for循环会自动i++.
			}
			else {
				lps[i] = 0;
			}
		}
	}
}

int Solution::compare(const std::vector<int>& lps, const std::string& pattern, const std::string& text)
{
	int sizeP = static_cast<int>(pattern.size());
	int sizeT = static_cast<int>(text.size());

	int j = 0;
	for (int i = 0; i < sizeT; i++) {
		while (j > 0 && text[i] != pattern[j]) {
			j = lps[j - 1];
		}
		if (text[i] == pattern[j]) {
			j++;
		}
		if (j == sizeP) {
			return i - j + 1;
		}
	}
	return -1;
}





