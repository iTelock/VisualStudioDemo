// minimum_sum_of_descent_path_v2.cpp: 定义应用程序的入口点。
//

#include "minimum_sum_of_descent_path_v2.h"

using namespace std;

int main()
{
	cout << "QS1289. 下降路径最小和 II (困难)." << endl;
	std::vector<std::vector<int>> grid = {
		{1,2,3},
		{4,5,6},
		{7,8,9}
	};
	Solution s;
	std::cout << s.minFallingPathSum(grid) << std::endl;
	return 0;
}

int Solution::minFallingPathSum(std::vector<std::vector<int>>& grid)
{
	int size = grid.size();
	if (size == 0) return 0;

	int min_prev_1 = 0;
	int min_prev_2 = 0;
	int index_prev = -1;

	for (int i = 0; i < size; i++) {
		int min_cur_1 = INT_MAX;
		int min_cur_2 = INT_MAX;
		int index_cur = -1;
		// 找到最小和次小值.
		for (int j = 0; j < size; j++) {
			int val_cur = (j != index_prev ? min_prev_1 : min_prev_2) + grid[i][j];	// 当前位置可能的最小值.
			if (val_cur < min_cur_1) {
				min_cur_2 = min_cur_1;
				min_cur_1 = val_cur;
				index_cur = j;
			}
			else if (val_cur < min_cur_2) {
				min_cur_2 = val_cur;
			}
		}
		min_prev_1 = min_cur_1;
		min_prev_2 = min_cur_2;
		index_prev = index_cur;
	}

	return min_prev_1;
}

