// non_intersect_lines.cpp: 定义应用程序的入口点。
//

#include "non_intersect_lines.h"

using namespace std;

int main()
{
	cout << "QS1035. 不相交的线 (中等)." << endl;
	std::vector<int> nums1 = { 1,4,2 };
	std::vector<int> nums2 = { 1,2,4 };
	Solution s;
	cout << s.maxUncrossedLines(nums1, nums2) << endl;
	return 0;
}

int Solution::maxUncrossedLines(std::vector<int>& nums1, std::vector<int>& nums2)
{
	int size1 = nums1.size();
	int size2 = nums2.size();

	if (size2 > size1) {
		std::swap(nums1, nums2);
		std::swap(size1, size2);
	}

	std::vector<int> dp(size2, 0);

	for (int i = 0; i < size1; i++) {
		int prev = 0;
		for (int j = 0; j < size2; j++) {
			int temp = dp[j];
			if (nums1[i] == nums2[j]) {
				dp[j] = prev + 1;
			}
			else if (j > 0) {
				dp[j] = std::max(dp[j], dp[j - 1]);
			}
			prev = temp;
		}
	}

	return dp[size2 - 1];
}
