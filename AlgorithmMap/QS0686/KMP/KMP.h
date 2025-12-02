// KMP.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>
#include <string>
#include <vector>

class Solution {
public:
    int repeatedStringMatch(std::string a, std::string b);
    bool kmpFunc(const std::string& text, const std::string& pattern);
    void kmpBuild(std::vector<int>& lps, const std::string& pattern);
    int kmpMatch(const std::vector<int>& lps, const std::string& text, const std::string& pattern);
};

// TODO: 在此处引用程序需要的其他标头。
