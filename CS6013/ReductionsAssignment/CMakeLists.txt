cmake_minimum_required(VERSION 3.27)
project(ReductionsAssignment)

set(CMAKE_CXX_STANDARD 17)
# Find OpenMP
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_executable(ReductionsAssignment main.cpp
        ParallelSum.cpp
)
