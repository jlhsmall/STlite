# cmake_minimum_required(VERSION <specify CMake version here>)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

set(src_dir
        main.cpp
        priority_queue.hpp exceptions.hpp class-bint.hpp class-integer.hpp class-matrix.hpp map.hpp utility.hpp)

add_executable(untitled ${src_dir})