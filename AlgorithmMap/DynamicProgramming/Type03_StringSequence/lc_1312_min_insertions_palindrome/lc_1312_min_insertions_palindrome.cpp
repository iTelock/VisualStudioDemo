// lc_1312_min_insertions_palindrome.cpp: 定义应用程序的入口点。
//

#include "lc_1312_min_insertions_palindrome.h"

using namespace std;

int main()
{
	cout << "1312. 让字符串成为回文串的最少插入次数." << endl;
	Solution so;
	std::string s = "mbadm";	// 2.
	std::cout << so.minInsertionsPlus(s) << std::endl;

	return 0;
}


int Solution::minInsertions(std::string s)
{
	int n = s.size();
	if (n <= 1) return 0;

	std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));

	for (int len = 2; len <= n; len++) {
		for (int i = 0; i + len - 1 < n; i++) {
			int j = i + len - 1; // 子串终点.
			if (s[i] == s[j]) {
				if (i + 1 <= j - 1) {
					dp[i][j] = dp[i + 1][j - 1];
				}
				else {
					dp[i][j] = 0;	// 去掉首尾只有1或0个字符，已经构成回文.
				}
			}
			else {
				dp[i][j] = std::min(dp[i + 1][j], dp[i][j - 1]) + 1;
			}
		}
	}

	return dp[0][n - 1];
}

// 一维数组空间优化.
int Solution::minInsertionsPlus(std::string s)
{
	int n = s.size();
	if (n <= 1) return 0;

	std::vector<int> dp(n, 0);

	// 遍历逻辑变了，不是按照len长度，而是按照二维dp，注意遍历顺序的先后.
	for (int i = n - 2; i >= 0; i--) {	// 反过来遍历，二维dp的下一行实际上是以为dp的上一行.
		int prev = 0;
		for (int j = i + 1; j < n; j++) {
			int tmp = dp[j];
			if (s[i] == s[j]) {
				// dp[i][j] = dp[i+1][j-1].
				dp[j] = prev;
			}
			else {
				//dp[i][j] = std::min(dp[i + 1][j], dp[i][j - 1]) + 1;
				dp[j] = std::min(dp[j], dp[j - 1]) + 1;
			}
			prev = tmp;
		}
	}

	return dp[n - 1];
}