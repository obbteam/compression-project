//
// Created by obbte on 09.12.2024.
//

#ifndef BINTREE_H
#define BINTREE_H
#include <cstdint>
#include <vector>
#include <memory>


class Node {
public:
    explicit Node(const uint8_t &data, int freq);

    Node(int freq, std::unique_ptr<Node> left, std::unique_ptr<Node> right);

    int get_freq() const;

    uint8_t* get_data();

    Node *left() const;

    Node *right() const;

    ~Node() = default;

private:
    std::unique_ptr<Node> m_left;
    std::unique_ptr<Node> m_right;

    std::unique_ptr<uint8_t> m_data;
    int m_frequency;
};


class BinTree {
public:
    explicit BinTree(const std::vector<std::pair<uint8_t, int> > &list);

    ~BinTree() = default;

    Node *build_tree();

    void print_tree();

    void print_root(const Node *root);

    Node *get_root() const;

private:
    Node *m_root;
    std::vector<std::unique_ptr<Node> > m_list{};
};


#endif //BINTREE_H
