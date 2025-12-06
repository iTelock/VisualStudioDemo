// lc_96_num_BST.cpp: 定义应用程序的入口点。
//

#include "lc_96_num_BST.h"

using namespace std;

int main()
{
	cout << "QS96_中等_不同的二叉搜索树.md." << endl;
	int n = 3;
	Solution s;
	std::cout << s.numTrees(n) << std::endl;
	return 0;
}

int Solution::numTrees(int n)
{
	std::vector<int> dp(n + 1);
	dp[0] = 1;		// 空树.
	dp[1] = 1;

	for (int i = 2; i <= n; i++) {
		for (int j = 1; j <= i; j++) {
			dp[i] += dp[j - 1] * dp[i - j];
		}
	}

	return dp[n];
}


