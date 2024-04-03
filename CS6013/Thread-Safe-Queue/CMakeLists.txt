cmake_minimum_required(VERSION 3.27)
project(Thread_Safe_Queue)

set(CMAKE_CXX_STANDARD 17)

add_executable(Thread_Safe_Queue main.cpp
        SerialQueueTests.cpp SerialQueue.hpp
        ConcurrentQueue.h
        TestConcurrentQueue.cpp)
