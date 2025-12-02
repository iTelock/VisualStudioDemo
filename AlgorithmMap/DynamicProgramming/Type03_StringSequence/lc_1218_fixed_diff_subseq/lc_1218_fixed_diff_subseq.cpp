// lc_1218_fixed_diff_subseq.cpp: 定义应用程序的入口点。
//

#include "lc_1218_fixed_diff_subseq.h"

using namespace std;

int main()
{
	cout << "1218. 最长定差子序列_中等" << endl;
	std::vector<int> arr = { 1, 2, 3, 4 };
	int difference = 1;		// 4.
	//std::vector<int> arr = { 1,5,7,8,5,3,4,2,1 };
	//int difference = -2;		// 4.
	Solution s;
	cout << s.longestSubsequence(arr, difference) << endl;
	return 0;
}

int Solution::longestSubsequence(std::vector<int>& arr, int difference)
{
	std::unordered_map<int, int> dp;
	int ans = 1;

	for (auto i : arr) {
		dp[i] = dp[i - difference] + 1;	// unordered_map对于未创建的自动为 0.
		if (dp[i] > ans) ans = dp[i];
	}

	return ans;
}
