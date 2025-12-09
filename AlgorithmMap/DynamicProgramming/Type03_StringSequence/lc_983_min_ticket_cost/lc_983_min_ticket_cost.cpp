// lc_983_min_ticket_cost.cpp: 定义应用程序的入口点。
//

#include "lc_983_min_ticket_cost.h"

using namespace std;

int main()
{
	cout << "983_中等_最低票价." << endl;
	std::vector<int> days = { 1,4,6,7,8,20 };
	std::vector<int> costs = { 2,7,15 };
	Solution s;
	std::cout << s.mincostTickets(days, costs) << std::endl;
	return 0;
}

int Solution::mincostTickets(std::vector<int>& days, std::vector<int>& costs)
{
	int n = days.size();
	int last_day = days.back();

	std::vector<bool> is_travel(last_day + 1, false);
	std::vector<int> dp(last_day + 1, 0);

	for (auto d : days) {
		is_travel[d] = true;
	}

	for (int i = 1; i <= last_day; i++) {
		if (is_travel[i]) {
			// 第 i 天旅行.
			int cost_day = dp[i - 1] + costs[0];
			int cost_week = dp[std::max(0, i - 7)] + costs[1];
			int cost_month = dp[std::max(0, i - 30)] + costs[2];
			dp[i] = std::min(std::min(cost_day, cost_week), cost_month);
		}
		else {
			// 第 i 天不旅行，等同于前一天.
			dp[i] = dp[i - 1];
		}
	}

	return dp[last_day];
}
