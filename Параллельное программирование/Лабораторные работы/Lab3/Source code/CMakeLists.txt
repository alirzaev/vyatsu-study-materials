cmake_minimum_required(VERSION 3.9)
project(Lab3)

enable_language(CXX)

# OpenMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

# optimizing
# Unroll loops +5%
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -funroll-loops")

set(CMAKE_CXX_STANDARD 17)

add_executable(Lab3 main.cpp algo.h)