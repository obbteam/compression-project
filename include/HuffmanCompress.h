//
// Created by Марк on 02.02.2025.
//

#ifndef COMPRESSION_PROJECT_HUFFMANCOMPRESS_H
#define COMPRESSION_PROJECT_HUFFMANCOMPRESS_H

#include "fstream"
#include "BitBuffer.h"
#include "string"
#include "HuffmanDictionary.h"
#include "HuffmanProperties.h"

class HuffmanCompress {

public:
    // Constructor
    HuffmanCompress(std::ifstream& in_file, std::ofstream& out_file, BitBuffer& bitBuffer, HuffmanDictionary& hd, HuffmanProperties& hp);

    // Main function
    void compress_file();

    // Helper functions
    void write_header();
    void write_dictionary();
    void write_encoded_data();
    void update_header();

    // Get functions
    uint16_t getPrepSize() const {return m_prep_size;}
    uint8_t getDictAmount() const {return m_dict_amount;}
    uint64_t getBitsAmount() const {return m_bits_amount;}

private:
    std::ofstream& file;
    std::ifstream& m_file;
    BitBuffer& bb;
    uint16_t m_prep_size = 0;
    uint8_t m_dict_amount = 0;
    uint64_t m_bits_amount = 0;

    std::string m_extension;
    std::unordered_map<uint8_t, std::string> m_encoded{};
};


#endif //COMPRESSION_PROJECT_HUFFMANCOMPRESS_H
