cmake_minimum_required(VERSION 3.22)
project(compression_project)

set(CMAKE_CXX_STANDARD 20)

add_executable(compression_project
        main.cpp
        src/Parser.cpp
        src/huffman.cpp
        src/BinTree.cpp
        src/BitBuffer.cpp
        src/LZW.cpp
        src/HuffmanProperties.cpp
        include/HuffmanProperties.h
        src/HuffmanDictionary.cpp
        include/HuffmanDictionary.h
        src/HuffmanCompress.cpp
        include/HuffmanCompress.h
        src/HuffmanDecompress.cpp
        include/HuffmanDecompress.h
)