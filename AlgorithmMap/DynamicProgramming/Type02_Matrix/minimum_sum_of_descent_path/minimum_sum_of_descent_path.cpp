// minimum_sum_of_descent_path.cpp: 定义应用程序的入口点。
//

#include "minimum_sum_of_descent_path.h"

using namespace std;

int main()
{
	cout << "QS931. 下降路径最小和." << endl;
	Solution s;
	std::vector<std::vector<int>> matrix = {
		{2,1,3},
		{6,5,4},
		{7,8,9}
	};	// 13.

	std::cout << s.minFallingPathSum(matrix) << std::endl;
	return 0;
}

int Solution::minFallingPathSum(std::vector<std::vector<int>>& matrix)
{
	int size = matrix.size();
	if (size == 0) return 0;

	std::vector<int> dp(size, 0);;
	std::vector<int> next_dp(size, 0);

	for (int i = 0; i < size; i++) {
		dp[i] = matrix[0][i];
	}

	for (int i = 1; i < size; i++) {
		next_dp[0] = std::min(dp[0], dp[1]) + matrix[i][0];
		for (int j = 1; j < size - 1; j++) {
			next_dp[j] = std::min(std::min(dp[j - 1], dp[j]), dp[j + 1]) + matrix[i][j];
		}
		next_dp[size - 1] = std::min(dp[size - 2], dp[size - 1]) + matrix[i][size - 1];
		dp.swap(next_dp);
	}

	return *std::min_element(dp.begin(), dp.end());
}

// 扩展，如果把每一行的最优解存在matrix中，就可以 O(1) 原地动态规划.