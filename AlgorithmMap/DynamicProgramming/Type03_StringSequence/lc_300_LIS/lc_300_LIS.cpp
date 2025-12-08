// lc_300_LIS.cpp: 定义应用程序的入口点。
//

#include "lc_300_LIS.h"

using namespace std;

int main()
{
	cout << "300 最长递增子序列 (中等)." << endl;
	//std::vector<int> nums = { 10,9,2,5,3,7,101,18 };	// 4.
	std::vector<int> nums = { 1,3,6,7,9,4,10,5,6 };		// 6.
	Solution s;
	std::cout << s.lengthOfLISPlus(nums) << std::endl;
	
	return 0;
}

int Solution::lengthOfLIS(std::vector<int>& nums)
{
	int size = nums.size();
	if (size <= 1) return size;
	int ans = 1;	// dp下标和nums对应，但是最优解不一定以末尾数字结尾，所以需要在遍历过程中记录最优解.

	std::vector<int> dp(size, 1);	// 任意位置至少有一个LIS.

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < i; j++) {
			if (nums[i] > nums[j]) {
				dp[i] = std::max(dp[i], dp[j] + 1);
			}
		}
		ans = std::max(ans, dp[i]);	// 外循环每一轮会覆盖掉dp[i].
	}

	return ans;
}

// 二分 + 贪心 （优于动态规划）.
int Solution::lengthOfLISPlus(std::vector<int>& nums)
{
	// 可以把 tails 理解为：“各长度对应的最优结尾值（num）”，不是真实的子序列，也不具有子序列所要求的顺序约束.
	std::vector<int> tails;
	for (int num : nums) {
		auto it = std::lower_bound(tails.begin(), tails.end(), num);
		if (it == tails.end()) {	// lower_bound 二分查找.
			tails.push_back(num);	// 未找到比 num 更大的，说明可以扩展LIS长度.
		}
		else {
			// 找到 >= num 的，那么这个位置的LIS可以用更小的nums来替代.
			*it = num;
		}
	}
	return tails.size();
}


