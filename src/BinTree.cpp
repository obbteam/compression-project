//
// Created by obbte on 09.12.2024.
//

#include <utility>

#include "../include/BinTree.h"

#include <iostream>

Node::Node(const uint8_t &data, const int freq) : m_left{nullptr},
                                                  m_right{nullptr},
                                                  m_frequency{freq},
                                                  m_data(std::make_unique<uint8_t>(data)) {
}

Node::Node(int freq, std::unique_ptr<Node> left, std::unique_ptr<Node> right)
    : m_left{std::move(left)},
      m_right{std::move(right)},
      m_frequency{freq},
      m_data{nullptr} {
}

int Node::get_freq() const {
    return m_frequency;
}

Node *Node::left() const {
    return m_left.get();
}

Node *Node::right() const {
    return m_right.get();
}

uint8_t *Node::get_data() {
    return m_data.get();
}


BinTree::BinTree(const std::vector<std::pair<uint8_t, int> > &list) : m_root(nullptr) {
    for (auto &it: list) {
        auto node = std::make_unique<Node>(it.first, it.second);
        m_list.push_back(std::move(node));
    }

    m_root = build_tree();
}

// todo use heap to reach O(n*log n)
Node *BinTree::build_tree() {
    while (m_list.size() > 1) {
        auto node3 = std::make_unique<Node>(m_list[0]->get_freq() + m_list[1]->get_freq(), std::move(m_list[0]),
                                            std::move(m_list[1]));
        m_list.erase(m_list.begin(), m_list.begin() + 2);

        if (m_list.empty()) {
            m_list.insert(m_list.begin(), std::move(node3));
        } else {
            bool inserted = false;
            for (int i = 0; i < m_list.size(); i++) {
                if (m_list[i]->get_freq() > node3->get_freq()) {
                    m_list.insert(m_list.begin() + i, std::move(node3));
                    inserted = true;
                    break;
                }
            }
            if (!inserted) m_list.push_back(std::move(node3));
        }
    }
    return m_list[0].get();
}


void BinTree::print_tree() {
    auto node = m_root;
    print_root(node);
    std::cout << std::endl;
}

void BinTree::print_root(const Node *root) {
    if (root == nullptr) {
        return;
    };

    std::cout << root->get_freq() << " ";
    print_root(root->left());
    print_root(root->right());
}

Node *BinTree::get_root() const {
    return m_root;
}
