//
// Created by Марк on 02.02.2025.
//

#ifndef COMPRESSION_PROJECT_HUFFMANDECOMPRESS_H
#define COMPRESSION_PROJECT_HUFFMANDECOMPRESS_H


#include "fstream"
#include "../BitBuffer.h"
#include "string"
#include "HuffmanProperties.h"

class HuffmanDecompress {

public:
    // Constructor
    explicit HuffmanDecompress(std::ifstream& inFile);


    // Helper functions
    void read_header();
    void read_dictionary();
    std::string read_huffman_code(uint8_t encoded_size);
    void decode_file(std::ofstream &out_file);

    // Get functions
    uint16_t getPrepSize() const {return m_prep_size;}
    uint8_t getDictAmount() const {return m_dict_amount;}
    uint64_t getBitsAmount() const {return m_bits_amount;}
    std::string getExtension() const {return m_extension;}

private:
    std::ifstream& in_file;
    uint16_t m_prep_size = 0;
    uint8_t m_dict_amount = 0;
    uint64_t m_bits_amount = 0;

    std::string m_extension;
    std::unordered_map<std::string, uint8_t> m_decoded{};
};


#endif //COMPRESSION_PROJECT_HUFFMANDECOMPRESS_H
