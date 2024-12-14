//
// Created by obbte on 03.12.2024.
//

#include <iostream>
#include <algorithm>

#include "../include/BinTree.h"
#include "../include/huffman.h"

Huffman::Huffman(std::ifstream &file): m_file(std::move(file)), m_size{0} {
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
    }
    m_sorted = std::vector<std::pair<uint8_t, int> >(m_dictionary.begin(), m_dictionary.end());

    std::sort(m_sorted.begin(), m_sorted.end(),
              [](const std::pair<uint8_t, int> &a, const std::pair<uint8_t, int> &b) {
                  return a.second < b.second;
              });

    print_sorted();
    auto object = BinTree(m_sorted);
    object.print_tree();
    std::cout << std::endl;
    // todo 1 - traverse binary tree to encode a byte
    auto node = object.get_root();

    traverse_tree(node);

    print_encoded();

    // todo 2 - encode file in the .groza format
    // todo 3 - decoding using the dictionary in the file
}

void Huffman::print_dict() const {
    for (auto &it: m_dictionary) {
        std::cout << it.first << ": " << it.second << std::endl;
    }
}

void Huffman::print_encoded() const {
    std::cout << "Encoded dict: " << std::endl;
    for (auto &it: m_encoded) {
        std::cout << it.first << ": " << std::bitset<16>(it.second) << std::endl;
        std::cout << "   size-" << (it.second >> 8) << "   code-" << (it.second & 0x00FF) << std::endl;
    }
}

void Huffman::print_sorted() const {
    for (auto &it: m_sorted) {
        std::cout << it.first << ": " << it.second << std::endl;
    }
}


void Huffman::traverse_tree(Node *root_node) {
    if (root_node == nullptr) return;

    create_code(root_node, 0, 0x00);
}


void Huffman::create_code(Node *root_node, uint8_t length, uint8_t code) {
    if (root_node == nullptr) return;

    if (length == 0 && root_node->left() == nullptr && root_node->right() == nullptr) length++;

    if (root_node->get_data()) {
        auto data = root_node->get_data();
        auto it = m_dictionary.find(*data);
        if (it != m_dictionary.end()) {
            m_encoded.insert(std::make_pair(it->first, (length << 8) | (code)));
        }
    }
    // shift to the left once (creates 1 zero at the end)
    create_code(root_node->left(), length + 1, code << 1);
    // shift to the left and create one at the end
    create_code(root_node->right(), length + 1, (code << 1) | 1);
}


Huffman::~Huffman() {
    m_file.seekg(0, std::ios::beg);
}


int Huffman::get_size() const {
    return m_size;
}
