// different_paths.cpp: 定义应用程序的入口点。
//

#include "different_paths.h"

using namespace std;

int main()
{
	cout << "QS62 不同路径." << endl;
	int m = 3;	// 行数.
	int n = 7;	// 列数.
	Solution s;
	std::cout << s.uniquePathsPlus(m, n) << std::endl;

	return 0;
}

int Solution::uniquePaths(int m, int n)
{
	std::vector<std::vector<int>> dp(m, std::vector<int>(n, 0));

	// 第一行第一列均为 1.
	for (int i = 0; i < m; i++) {
		dp[i][0] = 1;
	}
	for (int j = 0; j < n; j++) {
		dp[0][j] = 1;
	}

	for (int i = 1; i < m; i++) {
		for (int j = 1; j < n; j++) {
			dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
		}
	}

	return dp[m - 1][n - 1];
}

// 空间优化版本.
int Solution::uniquePathsPlus(int m, int n)
{
	std::vector<int> dp(n, 1);

	for (int i = 1; i < m; i++) {
		for (int j = 1; j < n; j++) {
			dp[j] += dp[j - 1];
		}
	}

	return dp[n - 1];
}