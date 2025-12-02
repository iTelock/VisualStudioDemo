// different_paths_v2.cpp: 定义应用程序的入口点。
//

#include "different_paths_v2.h"

using namespace std;

int main()
{
	cout << "QS63 不同路径2." << endl;
	//std::vector<std::vector<int>> obstacleGrid = { 
	//	{0, 0, 0},
	//	{0, 1, 0},
	//	{0, 0, 0}
	//};
	std::vector<std::vector<int>> obstacleGrid = {
		{0},
		{1}
	};
	Solution s;
	std::cout << s.uniquePathsWithObstacles_plus(obstacleGrid) << std::endl;

	return 0;
}

int Solution::uniquePathsWithObstacles(std::vector<std::vector<int>>& obstacleGrid)
{
	if (obstacleGrid[0][0] == 1) return 0;

	int row = obstacleGrid.size();
	int col = obstacleGrid[0].size();
	std::vector<std::vector<int>> dp(row, std::vector<int>(col, 0));

	// 行列初始化.
	for (int i = 0; i < row; i++) {
		if (obstacleGrid[i][0] == 1) {
			dp[i][0] = 0;
		}
		else {
			dp[i][0] = 1;
		}
	}
	for (int j = 0; j < col; j++) {
		if (obstacleGrid[0][j] == 1) {
			std::fill(dp[0].begin() + j, dp[0].end(), 0);
			break;
		}
		else {
			dp[0][j] = 1;
		}
	}

	// dp求解.
	for (int i = 1; i < row; i++) {
		for (int j = 1; j < col; j++) {
			if (obstacleGrid[i][j] == 1) {
				dp[i][j] = 0;
			}
			else {
				dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
			}
		}
	}

	return dp[row - 1][col - 1];
}

// 空间优化版.
int Solution::uniquePathsWithObstacles_plus(std::vector<std::vector<int>>& obstacleGrid)
{
	if (obstacleGrid[0][0] == 1) return 0;

	int row = obstacleGrid.size();
	int col = obstacleGrid[0].size();
	std::vector<int> dp(col, 0);
	dp[0] = 1;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (obstacleGrid[i][j] == 1) {
				dp[j] = 0;
			}
			else if (j > 0) {
				dp[j] += dp[j - 1];
			}
		}
	}

	return dp[col - 1];
}

