// lc_1027_max_arith_subseq.cpp: 定义应用程序的入口点。
//

#include "lc_1027_max_arith_subseq.h"

using namespace std;

int main()
{
	cout << "1027. 最长等差数列_中等." << endl;
	std::vector<int> nums = { 9,4,7,2,10 };	// 3.
	Solution s;
	std::cout << s.longestArithSeqLengthPlus(nums) << std::endl;
	return 0;
}


int Solution::longestArithSeqLength(std::vector<int>& nums)
{
	int n = nums.size();
	if (n <= 2) return n;
	// dp[i][d] → value: 以nums[i]结尾，公差为 d 的最长等差子序列长度.
	// 一个 vector 维护一个nums的值；一个map维护可能的 公差 和 对应长度.
	std::vector<std::unordered_map<int, int>> dp(n);
	int ans = 2;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < i; j++) {
			int d = nums[i] - nums[j];
			int len = 2;	// 至少有2个数构成等差（0和1的情况开头已处理）.
			auto it_j = dp[j].find(d);
			if (it_j != dp[j].end()) {
				len = it_j->second + 1;	// 原有等差数列长度上 +1.
			}
			auto it_i = dp[i].find(d);
			if (it_i == dp[i].end() || len > dp[i][d]) {
				dp[i][d] = len;
			}

			if (ans < dp[i][d]) ans = dp[i][d];
		}
	}

	return ans;
}

// 官方题解.
int Solution::longestArithSeqLengthPlus(std::vector<int>& nums)
{
	// 注意，题目已保证nums[i] >= 0.
	auto [ele_min, ele_max] = std::minmax_element(nums.begin(), nums.end());
	int diff = *ele_max - *ele_min;	// 可能的公差范围;
	int ans = 1;
	for (int d = -diff; d <= diff; d++) {
		std::vector<int> dp(*ele_max + 1, -1);
		for (auto num : nums) {
			int prev = num - d;
			if (prev >= *ele_min && prev <= *ele_max && dp[prev] != -1) {
				dp[num] = std::max(dp[num], dp[prev] + 1);
				ans = std::max(dp[num], ans);
			}
			dp[num] = std::max(dp[num], 1);		// 如果已经遍历过，则长度至少为 1.
		}
	}

	return ans;
}