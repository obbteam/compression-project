//
// Created by User on 1/12/2025.
//

#ifndef COMPRESSION_PROJECT_LZW_H
#define COMPRESSION_PROJECT_LZW_H

#include <iostream>
#include "LZW_compress.h"
#include "LZW_decompress.h"


class lzw {
public:
    // Constructor/destructor
    explicit lzw(const std::string &file);
    ~lzw();

    // Get functions
    std::string get_extension();
    std::string get_filename();

    // General functions
    void compress();
    void decompress();


private:
    std::string m_filename;
    std::ifstream m_file;

    static void validate_files(std::ifstream& inFile, std::ofstream& outFile);
};


#endif //COMPRESSION_PROJECT_LZW_H
