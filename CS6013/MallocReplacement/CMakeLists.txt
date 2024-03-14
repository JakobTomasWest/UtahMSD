cmake_minimum_required(VERSION 3.27)
project(MallocReplacement)

set(CMAKE_CXX_STANDARD 17)

add_executable(MallocReplacement main.cpp
        hashTable.h
        hashTable.cpp
        MyMalloc.cpp)
