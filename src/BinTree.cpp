//
// Created by obbte on 09.12.2024.
//

#include <utility>

#include "../include/BinTree.h"

#include <iostream>

Node::Node(const int freq) : m_left{nullptr}, m_right{nullptr}, m_frequency{freq} {
}

Node::Node(int freq, Node *left, Node *right) : m_frequency{freq}, m_left{left}, m_right{right} {
}

int Node::get_freq() const {
    return m_frequency;
}

Node *Node::left() const {
    return m_left;
}

Node *Node::right() const {
    return m_right;
}


BinTree::BinTree(const std::vector<std::pair<uint8_t, int> > &list) : m_root(nullptr), m_list() {
    for (auto &it: list) {
        auto node = Node(it.second);
        m_list.push_back(&node);
    }

    m_root = build_tree();
}

Node *BinTree::build_tree() {
    while (m_list.size() > 1) {
        auto node3 = Node(m_list[0]->get_freq() + m_list[1]->get_freq(), m_list[0], m_list[1]);
        m_list.erase(m_list.begin(), m_list.begin() + 1);

        if (m_list.empty()) {
            m_list.insert(m_list.begin(), &node3);
        } else {
            for (int i = 0; i < m_list.size(); i++) {
                if (m_list[i]->get_freq() > node3.get_freq()) {
                    m_list.insert(m_list.begin() + i, &node3);
                    break;
                }
            }
        }
    }
    return m_list[0];
}

void BinTree::print_tree() {
    auto node = m_root;
    print_root(node);
}

void BinTree::print_root(Node *root) {
    if (root == nullptr) return;

    std::cout << root->get_freq() << std::endl;
    print_root(root->left());
    print_root(root->right());
}
