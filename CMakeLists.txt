# cmake_minimum_required(VERSION <specify CMake version here>)
project(RISCV_Simulator)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

add_executable(code merge.cpp)