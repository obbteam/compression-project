//
// Created by obbte on 03.12.2024.
//

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "string"
#include "HuffmanDictionary.h"
#include "HuffmanCompress.h"
#include "HuffmanProperties.h"
#include "HuffmanDecompress.h"

#endif //HUFFMAN_H

class Huffman {
public:
    // (De)constructor
    explicit Huffman(const std::string &file);
    ~Huffman();

    // High-level methods for compression and decompression.
    void compress();
    void decompress();

    // Print functions
    void print_file();
    std::string get_extension() {return m_huffman_properties.get_extension();}


private:
    std::ifstream in_file;
    std::unordered_map<std::string, uint8_t> m_decoded{};
    HuffmanDictionary m_huffman_dict = HuffmanDictionary(in_file);
    HuffmanProperties m_huffman_properties;
};

