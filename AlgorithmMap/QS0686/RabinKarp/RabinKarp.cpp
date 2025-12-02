// RabinKarp.cpp: 定义应用程序的入口点。
//

#include "RabinKarp.h"

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
	int sizeA = a.size();
	int sizeB = b.size();
	int times = (sizeA + sizeB - 1) / sizeB;
	std::string text = a;
	for (int i = 1; i < times; i++) {
		text += a;
	}
	if (rabinKarpMatch(text, b)) {
		return times;
	}
	text += a;
	if (rabinKarpMatch(text, b)) {
		return times + 1;
	}
	return -1;
}

bool Solution::rabinKarpMatch(const std::string& a, const std::string& b)
{

}

