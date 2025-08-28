cmake_minimum_required(VERSION 3.27)
project(Lab03)

set(CMAKE_CXX_STANDARD 17)

add_executable(Lab03 main.cpp
        cmdline.cpp
        expr.cpp
        TESTS.cpp)
