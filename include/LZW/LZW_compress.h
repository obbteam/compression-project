//
// Created by Марк on 02.02.2025.
//

#ifndef COMPRESSION_PROJECT_LZW_COMPRESS_H
#define COMPRESSION_PROJECT_LZW_COMPRESS_H

#include "fstream"
#include "../BitBuffer.h"
#include "string"
#include "iostream"
#include <unordered_map>


class LZW_compress {

public:
    //Constructor
    LZW_compress(std::ifstream& inFile, std::string  extension);


    // Helper functions
    void write_file_metadata(std::ofstream& out_file);
    void encode_data(BitBuffer &bitBuffer);
    static void write_code(BitBuffer &bitBuffer, uint16_t output_code);
    void reset_dictionary(uint16_t &dict_size);
    void finalize_encoding(BitBuffer &bitBuffer);


private:
    std::ifstream& in_file;
    std::string m_extension;
    std::unordered_map<std::string, int> m_encoded;
    int MAX_DICT_SIZE = 4096;

    void fill_encoded();
};


#endif //COMPRESSION_PROJECT_LZW_COMPRESS_H
