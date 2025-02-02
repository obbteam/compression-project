cmake_minimum_required(VERSION 3.22)
project(compression_project)

set(CMAKE_CXX_STANDARD 20)

add_executable(compression_project
        main.cpp
        src/Parser.cpp
        src/Huffman/Huffman.cpp
        src/BinTree.cpp
        src/BitBuffer.cpp
        src/LZW/LZW.cpp
        src/Huffman/HuffmanProperties.cpp
        include/Huffman/HuffmanProperties.h
        src/Huffman/HuffmanDictionary.cpp
        include/Huffman/HuffmanDictionary.h
        src/Huffman/HuffmanCompress.cpp
        include/Huffman/HuffmanCompress.h
        src/Huffman/HuffmanDecompress.cpp
        include/Huffman/HuffmanDecompress.h
        src/LZW/LZW_compress.cpp
        include/LZW/LZW_compress.h
        src/LZW/LZW_decompress.cpp
        include/LZW/LZW_decompress.h
)