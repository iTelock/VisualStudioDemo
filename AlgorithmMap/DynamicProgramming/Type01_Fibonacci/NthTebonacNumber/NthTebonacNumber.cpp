// NthTebonacNumber.cpp: 定义应用程序的入口点。
//

#include "NthTebonacNumber.h"

using namespace std;

int main()
{
	cout << "QS1137. 第 N 个泰波那契数." << endl;
	Solution s;
	cout << s.tribonacci(37) << endl;
	return 0;
}

int Solution::tribonacci(int n)
{
	if (n == 0 || n == 1) {
		return n;
	}
	if (n == 2) {
		return 1;
	}
	int prev3 = 0;
	int prev2 = 1;
	int prev1 = 1;
	int current = 0;

	for (int i = 3; i <= n; i++) {
		current = prev1 + prev2 + prev3;
		prev3 = prev2;
		prev2 = prev1;
		prev1 = current;
	}

	return current;
}
