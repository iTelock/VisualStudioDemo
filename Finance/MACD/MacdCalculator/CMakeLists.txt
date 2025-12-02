cmake_minimum_required(VERSION 3.10)
project(MacdCalculator)

# 设置C++标准为17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含头文件目录
include_directories(include)

# 添加可执行文件，包含所有源文件
add_executable(MacdCalculator 
    src/MacdCalculator.cpp 
    main.cpp
 "src/MacdManager.cpp" "include/MacdManager.h")

# 设置编译器选项
if(MSVC)
    target_compile_options(MacdCalculator PRIVATE /W4)
else()
    target_compile_options(MacdCalculator PRIVATE -Wall -Wextra)
endif()