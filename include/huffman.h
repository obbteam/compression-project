//
// Created by obbte on 03.12.2024.
//

#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <fstream>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <iostream>
#include <algorithm>
#include "BinTree.h"
#include "BitBuffer.h"


#endif //HUFFMAN_H

class Huffman {
public:

    // Constructor/deconstructor
    Huffman(std::ifstream &file);

    ~Huffman();



    // Acsessors and printers
    void print_dict() const;

    void print_sorted() const;

    void print_encoded() const;

    int get_size() const;



    // General functions
    void compress(std::ofstream& file);

    void decompress(std::ofstream& file);



    // Helper functions
    void build_frequency_table();

    void sort_frequency_table();

    void build_huffman_tree();

    void encode_characters(Node *root_node);

private:
    int m_size;
    std::ifstream m_file;
    std::unordered_map<uint8_t, int> m_dictionary{};
    std::vector<std::pair<uint8_t, int> > m_sorted;
    std::unordered_map<uint8_t, uint16_t> m_encoded{};

    void create_code(Node *root_node, uint8_t length, uint8_t code);
};
