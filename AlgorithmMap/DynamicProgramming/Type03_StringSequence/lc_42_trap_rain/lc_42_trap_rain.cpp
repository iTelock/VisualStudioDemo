// lc_42_trap_rain.cpp: 定义应用程序的入口点。
//

#include "lc_42_trap_rain.h"

using namespace std;

int main()
{
	cout << "QS42_困难_接雨水." << endl;
	std::vector<int> height = { 0,1,0,2,1,0,1,3,2,1,2,1 };	// 6.
	Solution s;
	std::cout << s.trap(height) << std::endl;
	return 0;
}

int Solution::trap(std::vector<int>& height)
{
	int n = height.size();
	if (n <= 2) return 0;	// 少于3段不可能接到水.

	std::vector<int> left_max(n, 0);
	std::vector<int> right_max(n, 0);
	left_max[0] = height[0];
	right_max[n - 1] = height[n - 1];

	for (int i = 1; i < n; i++) {
		// 要么是之前的最高纪录，要么是当下最高.
		left_max[i] = std::max(left_max[i - 1], height[i]);
	}
	for (int i = n - 2; i >= 0; i--) {
		right_max[i] = std::max(right_max[i + 1], height[i]);
	}

	int sum_water = 0;
	for (int i = 1; i < n; i++) {
		sum_water += std::max(0, std::min(left_max[i], right_max[i]) - height[i]);
	}

	return sum_water;
}

