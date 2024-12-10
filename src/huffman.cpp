//
// Created by obbte on 03.12.2024.
//

#include <iostream>
#include <algorithm>

#include "../include/BinTree.h"
#include "../include/huffman.h"

Huffman::Huffman(std::ifstream &file): m_file(std::move(file)), m_dictionary() {
    while (m_file.peek() != EOF) {
        uint8_t byte = m_file.get();
        ++m_size;

        auto result = m_dictionary.find(byte);
        if (result == m_dictionary.end()) {
            // frequency is 1
            m_dictionary.insert(std::make_pair(byte, 1));
        } else {
            // increment the frequency of the byte in the dict
            result->second++;
        }
        m_sorted = std::vector<std::pair<uint8_t, int> >(m_dictionary.begin(), m_dictionary.end());

        std::sort(m_sorted.begin(), m_sorted.end(),
                  [](const std::pair<uint8_t, int> &a, const std::pair<uint8_t, int> &b) {
                      return a.second < b.second;
                  });
    }

    print_sorted();
    auto object = BinTree(get_sorted());
    object.print_tree();
}

void Huffman::print_dict() const {
    for (auto &it: m_dictionary) {
        std::cout << it.first << ": " << it.second << std::endl;
    }
}

void Huffman::print_sorted() const {
    for (auto &it: m_sorted) {
        std::cout << it.first << ": " << it.second << std::endl;
    }
}

std::vector<std::pair<uint8_t, int> > Huffman::get_sorted() const {
    return m_sorted;
}


Huffman::~Huffman() {
    m_file.seekg(0, std::ios::beg);
}


int Huffman::get_size() const {
    return m_size;
}
