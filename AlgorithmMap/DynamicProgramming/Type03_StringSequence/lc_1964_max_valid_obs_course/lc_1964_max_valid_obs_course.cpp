// lc_1964_max_valid_obs_course.cpp: 定义应用程序的入口点。
//

#include "lc_1964_max_valid_obs_course.h"

using namespace std;

int main()
{
	cout << "1964_困难_找出到每个位置为止最长的有效障碍赛跑路线." << endl;
	Solution s;
	std::vector<int> obstacles = { 1, 2, 3, 2 };
	std::vector<int> result = s.longestObstacleCourseAtEachPosition(obstacles);
	for (auto i : result) {
		std::cout << i << ", ";
	}
	std::cout << std::endl;
	return 0;
}

std::vector<int> Solution::longestObstacleCourseAtEachPosition(std::vector<int>& obstacles)
{
	int n = obstacles.size();
	std::vector<int> tails;
	std::vector<int> ans(n);

	for (int i = 0; i < n; i++) {
		auto it = std::upper_bound(tails.begin(), tails.end(), obstacles[i]);
		ans[i] = static_cast<int>(it - tails.begin()) + 1;	// 仅需要计算以 i 结尾.
		// 接下来更新 tails 辅助计算后续ans[i].
		if (it == tails.end()) {
			tails.push_back(obstacles[i]);	// 没有比 obstacles[i] 更大的，可以扩展长度
		}
		else {
			*it = obstacles[i];	// 有更大的，替换为更小的 (贪心).
		}
	}

	return ans;
}
