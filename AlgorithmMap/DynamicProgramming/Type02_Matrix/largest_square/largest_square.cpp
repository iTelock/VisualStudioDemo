// largest_square.cpp: 定义应用程序的入口点。
//

#include "largest_square.h"

using namespace std;

int main()
{
	cout << "QS221. 最大正方形 （中等）." << endl;
	std::vector<std::vector<char>> matrix = {
		{'1','0','1','0','0'},
		{'1','0','1','1','1'},
		{'1','1','1','1','1'},
		{'1','0','0','1','0'}
	};	// 4.

	Solution s;
	std::cout << s.maximalSquare(matrix) << std::endl;

	return 0;
}

int Solution::maximalSquare(std::vector<std::vector<char>>& matrix)
{
	int rows = matrix.size();
	if (rows == 0) return 0;
	int cols = matrix[0].size();
	int max = 0;

	std::vector<std::vector<int>> dp(rows + 1, std::vector<int>(cols + 1, 0));	// matrix[i-1][j-1] 对应 dp[i][j].
	for (int i = 1; i <= rows; i++) {
		for (int j = 1; j <= cols; j++) {
			if (matrix[i - 1][j - 1] == '0') {
				dp[i][j] = 0;
			}
			else {	// 字符为 1 , 考虑扩展.
				dp[i][j] = std::min({
					dp[i - 1][j],	// 上方.
					dp[i][j - 1],	// 左侧.
					dp[i - 1][j - 1]	// 左上方.
				}) + 1;
				max = std::max(max, dp[i][j]);
			}
		}
	}

	return max * max;
}

// 一维数组空间优化 O(n).
int Solution::maximalSquarePlus(std::vector<std::vector<char>>& matrix)
{

}