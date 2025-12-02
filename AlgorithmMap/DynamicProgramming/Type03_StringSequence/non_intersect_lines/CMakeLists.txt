# CMakeList.txt: non_intersect_lines 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("non_intersect_lines")

# 将源代码添加到此项目的可执行文件。
add_executable (non_intersect_lines "non_intersect_lines.cpp" "non_intersect_lines.h")

# TODO: 如有需要，请添加测试并安装目标。
