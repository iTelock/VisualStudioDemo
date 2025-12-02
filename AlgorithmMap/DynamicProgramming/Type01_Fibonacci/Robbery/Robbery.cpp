// Robbery.cpp: 定义应用程序的入口点。
//

#include "Robbery.h"

using namespace std;

int main()
{
	cout << "QS198. 打家劫舍." << endl;
	Solution s;
	//std::vector<int> nums = { 1,2,3,1 };
	std::vector<int> nums = { 2,7,9,3,1 };
	std::cout << s.rob(nums) << std::endl;

	return 0;
}

int Solution::rob(std::vector<int>& nums)
{
	int size = nums.size();
	if (size == 1) {
		return nums[0];
	}
	if (size == 2) {
		return std::max(nums[0], nums[1]);
	}

	int prev2 = nums[0];						// dp[0].
	int prev1 = std::max(nums[0], nums[1]);		// dp[1].
	int current = 0;
	for (int i = 2; i < size; i++) {
		current = std::max(prev2 + nums[i], prev1);
		prev2 = prev1;
		prev1 = current;
	}

	return current;
}
