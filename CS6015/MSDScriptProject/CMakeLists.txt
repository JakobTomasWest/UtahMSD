cmake_minimum_required(VERSION 3.27)
project(Lab03)

set(CMAKE_CXX_STANDARD 17)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")

add_executable(Lab03 main.cpp
        catch.hpp
        parse.cpp
        cmdline.cpp
        expr.cpp
        TESTS.cpp
        Val.h
        val.cpp
        parse.cpp)
