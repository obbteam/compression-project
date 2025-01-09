cmake_minimum_required(VERSION 3.22)
project(compression_project)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Debug)

add_executable(compression_project
        main.cpp
        src/Parser.cpp
        src/huffman.cpp
        src/BinTree.cpp
        src/BitBuffer.cpp
)