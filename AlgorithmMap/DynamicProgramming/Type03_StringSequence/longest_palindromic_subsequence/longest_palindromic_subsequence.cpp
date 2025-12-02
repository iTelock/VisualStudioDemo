// longest_palindromic_subsequence.cpp: 定义应用程序的入口点。
//

#include "longest_palindromic_subsequence.h"

using namespace std;

int main()
{
	cout << "QS516 最长回文子序列  (中等)." << endl;
	std::string s = "bbbab";
	Solution so;
	std::cout << so.longestPalindromeSubseqPlus(s) << std::endl;
	return 0;
}

int Solution::longestPalindromeSubseq(std::string s)
{
	int size = s.size();
	if (size == 0) return 0;

	std::vector<std::vector<int>> dp(size, std::vector<int>(size, 0));

	// 单个字符ok.
	for (int i = 0; i < size; i++) {
		dp[i][i] = 1;
	}

	for (int len = 2; len <= size; len++) {	// 长度从2扩展为size.
		for (int i = 0; i + len - 1 < size; i++) {	// 从头开始检查对应长度的dp.
			int j = i + len - 1;	// 区间结尾.
			if (s[i] == s[j]) {
				if (len == 2) {
					dp[i][j] = 2;
				}
				else {
					dp[i][j] = dp[i + 1][j - 1] + 2;
				}
			}
			else {
				dp[i][j] = std::max(dp[i + 1][j], dp[i][j - 1]);
			}
		}
	}

	return dp[0][size - 1];
}

// 一维数组空间优化.
int Solution::longestPalindromeSubseqPlus(std::string s)
{
	int size = s.size();
	if (size == 0) return 0;
	string t = s;
	reverse(t.begin(), t.end());

	std::vector<int> dp(size, 0);

	// 问题转化为寻找 s 和它自己反转的 t 的LCS（最长公共子序列）.
	for (int i = 0; i < size; i++) {
		int prev = 0;	// dp[i-1][j-1].
		for (int j = 0; j < size; j++) {
			int temp = dp[j];	// // 先保存 dp[i-1][j].
			if (s[i] == t[j]) {
				dp[j] = prev + 1;
			}
			else if(j > 0) {
				dp[j] = std::max(dp[j], dp[j - 1]);
			}
			prev = temp;
		}
	}

	return dp[size - 1];
}
