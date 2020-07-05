cmake_minimum_required(VERSION 3.9)
project(Lab1)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -funroll-loops")

add_executable(Trivial main_trivial.cpp algo.h)
add_executable(Improved main_improved.cpp algo.h)
add_executable(Checker main_checker.cpp algo.h)