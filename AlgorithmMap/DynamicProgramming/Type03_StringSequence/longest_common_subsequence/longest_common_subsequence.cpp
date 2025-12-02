// longest_common_subsequence.cpp: 定义应用程序的入口点。
//

#include "longest_common_subsequence.h"

using namespace std;

int main()
{
	cout << "QS1143最长公共子序列（中等）." << endl;
	string text1 = "abcde";
	string text2 = "ace";

	Solution s;
	std::cout << s.longestCommonSubsequencePlus(text1, text2) << std::endl;

	return 0;
}

int Solution::longestCommonSubsequence(std::string text1, std::string text2)
{
	int size1 = text1.size();
	int size2 = text2.size();

	std::vector<std::vector<int>> dp(size1 + 1, std::vector<int>(size2 + 1, 0));

	for (int i = 1; i <= size1; i++) {
		for (int j = 1; j <= size2; j++) {
			if (text1[i - 1] == text2[j - 1]) {
				dp[i][j] = dp[i - 1][j - 1] + 1;
			}
			else {
				dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
			}
		}
	}

	return dp[size1][size2];
}

int Solution::longestCommonSubsequencePlus(std::string text1, std::string text2)
{
	int size1 = text1.size();
	int size2 = text2.size();

	if (size2 > size1) {
		std::swap(text1, text2);
		std::swap(size1, size2);
	}

	std::vector<int> dp(size2 + 1, 0);

	for (int i = 1; i <= size1; i++) {
		int prev = 0;	// 对应 dp[i-1][j-1].
		for (int j = 1; j <= size2; j++) {
			int temp = dp[j];	// 暂存当前 dp[j]，这是 dp[i-1][j]（上方）.
			if (text1[i - 1] == text2[j - 1]) {
				dp[j] = prev + 1;
			}
			else {
				dp[j] = std::max(dp[j], dp[j - 1]);	// max(上, 左).
			}
			prev = temp;
		}
	}

	return dp[size2];
}
