//
// Created by obbte on 03.12.2024.
//

#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <fstream>
#include <unordered_map>

#endif //HUFFMAN_H

class Huffman {
public:
    Huffman(std::ifstream &file);

    ~Huffman();

    void print_dict() const;

private:
    std::ifstream m_file;
    std::unordered_map<uint8_t, int> m_dictionary;
};
