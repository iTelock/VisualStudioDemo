// delete_get_points.cpp: 定义应用程序的入口点。
//

#include "delete_get_points.h"

using namespace std;

int main()
{
	cout << "QS740. 删除并获得点数." << endl;
	std::vector<int> nums = { 3, 4, 2 };
	//std::vector<int> nums = { 2,2,3,3,3,4 };
	Solution s;
	std::cout << s.deleteAndEarn(nums) << std::endl;

	return 0;
}

int Solution::deleteAndEarn(vector<int>& nums)
{
	if (nums.empty()) return 0;
	std::unordered_map<int, int> points;
	// 1. 统计累加 每个数字出现的积分
	for (auto num : nums) {
		points[num] += num;
	}
	// 2. 把出现过的 nums 数字排序
	std::vector<int> vals;
	vals.reserve(points.size());
	for (auto& val : points) {
		vals.push_back(val.first);
	}
	std::sort(vals.begin(), vals.end());
	// 3. dp 求解 （空间优化）
	int prev2 = 0;	// dp[0] = 0, dp[i-2].
	int prev1 = points[vals[0]];	// dp[1] = 一个数字时候.
	for (int i = 1; i < points.size(); i++) {
		int current;
		// 冲突检查.
		if (vals[i] == vals[i - 1] + 1) {	// 已排序，只需要检查+1.
			current = std::max(prev1, prev2 + points[vals[i]]);
		}
		else {
			current = prev1 + points[vals[i]];	// 不相邻可以累加积分.
		}
		prev2 = prev1;
		prev1 = current;
	}

	return prev1;
}