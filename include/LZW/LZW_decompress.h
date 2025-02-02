//
// Created by Марк on 02.02.2025.
//

#ifndef COMPRESSION_PROJECT_LZW_DECOMPRESS_H
#define COMPRESSION_PROJECT_LZW_DECOMPRESS_H



#include "fstream"
#include "../BitBuffer.h"
#include "string"
#include "iostream"
#include <unordered_map>


class LZW_decompress {

public:
    // Constructor
    LZW_decompress(std::ifstream& inFile);


    // Helper functions
    std::string read_extension();
    int read_code(BitBuffer &bitBuffer);
    void write_to_file(std::ofstream& out_file, const std::string &data);
    void process_compressed_data(BitBuffer &bitBuffer, std::string &cur, int &cur_code, int &dict_size, std::ofstream& out_file);
    std::string get_decoded_string(int cur_code, int next_code);
    void reset_dictionary(int &dict_size);
    void fill_decoded();

    // Get function
    std::unordered_map<int, std::string> get_decoded() const {return m_decoded;}


private:
    std::ifstream& in_file;
    std::unordered_map<int, std::string> m_decoded;

    int MAX_DICT_SIZE = 4096;
};


#endif //COMPRESSION_PROJECT_LZW_DECOMPRESS_H
