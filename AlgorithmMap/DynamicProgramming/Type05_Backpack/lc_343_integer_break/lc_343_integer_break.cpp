// lc_343_integer_break.cpp: 定义应用程序的入口点。
//

#include "lc_343_integer_break.h"

using namespace std;

int main()
{
	cout << "QS343_中等_整数拆分." << endl;
	Solution s;
	int n = 10;

	std::cout << s.integerBreak(n) << std::endl;
	return 0;
}

int Solution::integerBreak(int n)
{
	if (n <= 3) {
		return n - 1;
	}

	std::vector<int> dp(n + 1, 0);	// dp[i]: 数字i拆分后的最大乘积.
	dp[2] = 1;

	for (int i = 3; i <= n; i++) {
		for (int j = 1; j < i; j++) {
			int current_value = j * (i - j);
			int backtrack_value = j * dp[i - j];
			int max_value = std::max(current_value, backtrack_value);
			dp[i] = dp[i] < max_value ? max_value : dp[i];
		}
	}

	return dp[n];
}

// 可以用数学规律简化为 O(1) O(1).
