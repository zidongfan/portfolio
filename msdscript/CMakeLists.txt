cmake_minimum_required(VERSION 3.24)
project(msdscript)

set(CMAKE_CXX_STANDARD 17)

add_executable(msdscript main.cpp cmdline.cpp expr.cpp testExpr.cpp parse.cpp exec.cpp val.cpp env.cpp)
