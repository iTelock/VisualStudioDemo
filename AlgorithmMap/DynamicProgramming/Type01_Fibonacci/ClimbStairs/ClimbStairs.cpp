// ClimbStairs.cpp: 定义应用程序的入口点。
//

#include "ClimbStairs.h"

using namespace std;

int main()
{
	cout << "70. 爬楼梯." << endl;
	int n = 3;
	Solution s;
	std::cout << s.climbStairs(n) << std::endl;




	return 0;
}

int Solution::climbStairs(int n)
{
	if (n <= 1) {
		return 1;
	}
	std::vector<int> dp(n + 1, 0);
	dp[0] = 1;
	dp[1] = 1;

	// dp[i] 的方案数为 跨一步的dp[i-1] 和跨两步的dp[i-2] 方案数之和.
	for (int i = 2; i <= n; i++) {
		dp[i] = dp[i - 1] + dp[i - 2];
	}

	return dp[n];
}