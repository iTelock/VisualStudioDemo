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

}
