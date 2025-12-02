// lc_646_LIS_logarithmic_chain.cpp: 定义应用程序的入口点。
//

#include "lc_646_LIS_logarithmic_chain.h"

using namespace std;

int main()
{
	cout << "QS646 最长对数链 （中等）." << endl;
	std::vector<std::vector<int>> pairs = {
		{1,2},
		{2,3},
		{3,4}
	};	// 2.
	Solution s;
	auto start = std::chrono::high_resolution_clock::now();
	std::cout << s.findLongestChainPlus(pairs) << std::endl;
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << duration.count() << std::endl;

	return 0;
}

// 动态规划.
int Solution::findLongestChain(std::vector<std::vector<int>>& pairs)
{
	int size = pairs.size();
	if (size <= 1) return size;

	std::sort(pairs.begin(), pairs.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
		if (a[0] == b[0]) return a[1] < b[1];
		return a[0] < b[0];
	});

	std::vector<int> dp(size, 1);
	int ans = 1;

	// 每次固定 i 位置，然后看 i 左侧的数对能否匹配，逐步寻找这个位置上的 LIS.
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < i; j++) {
			if (pairs[i][0] > pairs[j][1]) {
				// 能组成数链，LIS增加.
				dp[i] = std::max(dp[i], dp[j] + 1);
			}
		}
		ans = std::max(ans, dp[i]);
	}

	return ans;
}

// 贪心 + 二分 （优于动态规划）.
int Solution::findLongestChainPlus(std::vector<std::vector<int>>& pairs)
{
	
	int size = pairs.size();
	if (size <= 1) return size;
	
	int end = INT_MIN;
	int ans = 0;

	// 按数对的结尾从小到大排序.
	std::sort(pairs.begin(), pairs.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
		return a[1] < b[1];
	});

	for (const auto& v : pairs) {
		if (v[0] > end) {
			ans++;
			end = v[1];
		}
	}

	return ans;
}
