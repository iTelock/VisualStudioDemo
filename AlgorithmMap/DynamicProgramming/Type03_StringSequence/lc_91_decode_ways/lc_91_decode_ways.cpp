// lc_91_decode_ways.cpp: 定义应用程序的入口点。
//

#include "lc_91_decode_ways.h"

using namespace std;

int main()
{
	cout << "91_中等_解码方法." << endl;
	std::string s = "226";	// 3.
	Solution so;
	std::cout << so.numDecodings(s) << std::endl;
	return 0;
}

int Solution::numDecodings(std::string s)
{
	if (s[0] == '0') return 0;

	int n = s.size();
	std::vector<int> dp(n + 1, 0);	// 前 i 个字符的解数量.
	dp[0] = 1;	// 空串默认有1个解.
	dp[1] = 1;	// 已经判断第一个字符为 0 的情况，这里判断合法时单字符有 1 个解.

	for (int i = 2; i <= n; i++) {
		if (s[i - 1] != '0') {
			dp[i] += dp[i - 1];
		}
		int digit = (s[i - 2] - '0') * 10 + (s[i - 1] - '0');
		if (10 <= digit && digit <= 26) {
			dp[i] += dp[i - 2];
		}
	}

	return dp[n];
}
