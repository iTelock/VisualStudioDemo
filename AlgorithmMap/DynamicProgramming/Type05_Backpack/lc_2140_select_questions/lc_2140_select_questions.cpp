// lc_2140_select_questions.cpp: 定义应用程序的入口点。
//

#include "lc_2140_select_questions.h"

using namespace std;

int main()
{
	cout << "QS2140_中等_解决智力问题." << endl;
	std::vector<std::vector<int>> questions = {
		{1,1},
		{2,2},
		{3,3},
		{4,4},
		{5,5}
	};
	Solution s;
	std::cout << s.mostPoints(questions) << std::endl;

	return 0;
}

long long Solution::mostPoints(std::vector<std::vector<int>>& questions)
{
	int n = questions.size();
	if (1 == n) {
		return questions[0][0];
	}

	std::vector<long long> dp(n + 1, 0);

	for (int i = n - 1; i >= 0; i--) {
		int next = i + questions[i][1] + 1;		// 获取本题下一题坐标.
		long long point = questions[i][0];		// 获取本题得分.
		next = next > n ? n : next;
		long long skip_point = dp[i + 1];			// 跳过本题后最优得分.
		long long take_point = dp[next] + point;	// 获取本题后最优得分.
		dp[i] = std::max(skip_point, take_point);
	}

	return dp[0];
}
