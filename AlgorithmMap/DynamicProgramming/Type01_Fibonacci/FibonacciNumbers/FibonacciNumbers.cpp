// FibonacciNumbers.cpp: 定义应用程序的入口点。
//

#include "FibonacciNumbers.h"

using namespace std;

int main()
{
	cout << "QS509 斐波那契数." << endl;
	Solution s;
	cout << s.fib(3) << endl;
	return 0;
}


int Solution::fib(int n)
{
	if (n == 1 || n == 0) {
		return n;
	}
	int prev1 = 1;
	int prev2 = 0;
	int current = 0;
	for (int i = 2; i <= n; i++) {
		current = prev1 + prev2;
		prev2 = prev1;
		prev1 = current;
	}

	return current;
}