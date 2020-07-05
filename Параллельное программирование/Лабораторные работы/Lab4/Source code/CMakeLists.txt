cmake_minimum_required(VERSION 3.10)
project(Lab4)

set(CMAKE_CXX_STANDARD 17)

find_package(MPI REQUIRED)

# optimizing
# Unroll loops +5%
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -funroll-loops")

add_executable(Lab4 main.cpp algo.h)

target_link_libraries(Lab4 PRIVATE ${MPI_C_LIBRARIES})

target_include_directories(Lab4 PRIVATE ${MPI_C_INCLUDE_PATH})

# mpirun -n 12 ./Lab4