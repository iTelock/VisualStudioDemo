// lc_2466_count_good_str.cpp: 定义应用程序的入口点。
//

#include "lc_2466_count_good_str.h"

using namespace std;

int main()
{
	cout << "QS2466_中等_统计构造好字符串的方案数." << endl;
	int low = 2, high = 3, zero = 1, one = 2;
	Solution s;
	std::cout << s.countGoodStrings(low, high, zero, one) << std::endl;
	return 0;
}

int Solution::countGoodStrings(int low, int high, int zero, int one)
{
	int kMod = 1e9 + 7;
	std::vector<int> dp(high + 1, 0);
	dp[0] = 1;	// 空串默认为一种解.

	long long ans = 0;  // 用 long long 避免中间加法溢出.

	for (int i = 1; i <= high; i++) {
		if (i >= zero) {
			dp[i] = (dp[i] + dp[i - zero]) % kMod;
		}

		if (i >= one) {
			dp[i] = (dp[i] + dp[i - one]) % kMod;
		}

		if (i >= low) {
			ans = (ans + dp[i]) % kMod;
		}
	}

	return static_cast<int>(ans);
}

