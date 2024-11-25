cmake_minimum_required(VERSION 3.29)
project(compression_project)

set(CMAKE_CXX_STANDARD 20)

add_executable(compression_project main.cpp
        src/Parser.cpp
        include/Parser.h)
