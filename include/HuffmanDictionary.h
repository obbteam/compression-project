//
// Created by Марк on 02.02.2025.
//

#ifndef COMPRESSION_PROJECT_HUFFMANDICTIONARY_H
#define COMPRESSION_PROJECT_HUFFMANDICTIONARY_H

#include "fstream"
#include "vector"
#include "map"
#include "string"
#include "BinTree.h"
#include "iostream"

class HuffmanDictionary {

public:
    // Constructor
    explicit HuffmanDictionary(std::ifstream& file);

    // Main function
    void create_encoded_dictionary();

    // Helper functions
    void build_frequency_table();
    void sort_frequency_table();
    void build_huffman_tree();
    void encode_characters(Node *root_node);


    // Print/get functions
    void print_dict() const;
    void print_sorted() const;
    void print_encoded() const;

    std::unordered_map<uint8_t, int> get_dict() const {return m_dictionary;}
    std::vector<std::pair<uint8_t, int> > get_sorted() const {return m_sorted;}
    std::unordered_map<uint8_t, std::string> get_encoded() const {return m_encoded;}
    int get_size() const {return m_size;}


private:
    int m_size;
    std::unordered_map<uint8_t, int> m_dictionary{};
    std::vector<std::pair<uint8_t, int> > m_sorted;
    std::unordered_map<uint8_t, std::string> m_encoded{};
    std::ifstream& m_file;

    void create_code(Node *root_node, uint8_t length, std::string &&code);
};


#endif //COMPRESSION_PROJECT_HUFFMANDICTIONARY_H
