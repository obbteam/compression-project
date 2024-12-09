//
// Created by obbte on 03.12.2024.
//

#include <iostream>

#include "../include/huffman.h"

Huffman::Huffman(std::ifstream &file): m_file(std::move(file)), m_dictionary() {
    while (m_file.peek() != EOF) {
        uint8_t byte = m_file.get();

        auto result = m_dictionary.find(byte);
        if (result == m_dictionary.end()) {
            // frequency is 1
            m_dictionary.insert(std::make_pair(byte, 1));
        } else {
            // increment the frequency of the byte in the dict
            result->second++;
        }
    }

    std::sort(m_dictionary.begin()->second, m_dictionary.end()->second);
}

void Huffman::print_dict() const {
    for (auto &it: m_dictionary) {
        std::cout << it.first << ": " << it.second << std::endl;
    }
}

Huffman::~Huffman() {
    m_file.seekg(0, std::ios::beg);
}
