//
// Created by User on 1/12/2025.
//

#ifndef COMPRESSION_PROJECT_LZW_H
#define COMPRESSION_PROJECT_LZW_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>

class lzw{

public:
    explicit lzw(const std::string &file);

    ~lzw();

    std::string get_extension();

    std::string get_filename();


    // General functions
    void compress();

    void decompress();

private:
    std::ifstream m_file;
    std::string m_filename;
    std::unordered_map<std::string, int> m_encode;
    std::unordered_map<int, std::string> m_decode;
};


#endif //COMPRESSION_PROJECT_LZW_H

