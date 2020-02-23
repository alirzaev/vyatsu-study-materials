cmake_minimum_required(VERSION 3.10)
project(ColorizeGraphMPI C)

set(CMAKE_C_STANDARD 99)

find_package(MPI REQUIRED)

# optimizing
# Unroll loops +5%
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -funroll-loops")

add_executable(ColorizeGraphMPI main.c algo.h algo.c)

target_link_libraries(ColorizeGraphMPI PRIVATE ${MPI_C_LIBRARIES})

target_include_directories(ColorizeGraphMPI PRIVATE ${MPI_C_INCLUDE_PATH})
