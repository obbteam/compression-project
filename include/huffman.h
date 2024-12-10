//
// Created by obbte on 03.12.2024.
//

#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <fstream>
#include <unordered_map>
#include <vector>


#endif //HUFFMAN_H

class Huffman {
public:
    Huffman(std::ifstream &file);

    ~Huffman();

    void print_dict() const;

    void print_sorted() const;

    // create tree

    // create file

    int get_size() const;

//    std::vector<std::pair<uint8_t, int> > get_sorted() const;

private:
    int m_size;
    std::ifstream m_file;
    std::unordered_map<uint8_t, int> m_dictionary;
    // BinTree m_tree;
    std::vector<std::pair<uint8_t, int> > m_sorted;
};
