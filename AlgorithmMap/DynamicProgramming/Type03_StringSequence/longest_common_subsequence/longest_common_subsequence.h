// longest_common_subsequence.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class Solution {
public:
    int longestCommonSubsequence(std::string text1, std::string text2);
    int longestCommonSubsequencePlus(std::string text1, std::string text2); //空间优化.
};

// TODO: 在此处引用程序需要的其他标头。
