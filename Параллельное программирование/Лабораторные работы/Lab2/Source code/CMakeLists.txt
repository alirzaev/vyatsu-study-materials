cmake_minimum_required(VERSION 3.9)
project(Lab2)

set(CMAKE_CXX_STANDARD 17)

add_executable(Generator main_generator.cpp algo.h)
add_executable(Improved main_improved.cpp algo.h)