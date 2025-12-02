// KMP.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    // 方法1：Rabin–Karp（多项式滚动哈希）+ 二次比对.
    int strStr(std::string haystack, std::string needle);
    void buildLPS(std::vector<int>& lps, const std::string& pattern);
    int compare(const std::vector<int>& lps, const std::string& pattern, const std::string& text);
};

// TODO: 在此处引用程序需要的其他标头。
