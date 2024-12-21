cmake_minimum_required(VERSION 3.22)
project(compression_project)

set(CMAKE_CXX_STANDARD 20)

add_executable(compression_project main.cpp
        src/Parser.cpp
        src/huffman.cpp
        src/BinTree.cpp
        include/BitBuffer.h
        src/BitBuffer.cpp
        include/Decode.h
        src/Decode.cpp
)
