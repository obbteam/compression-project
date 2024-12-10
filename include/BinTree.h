//
// Created by obbte on 09.12.2024.
//

#ifndef BINTREE_H
#define BINTREE_H
#include <cstdint>
#include <vector>


class Node {
public:
    explicit Node(int freq);

    Node(int freq, Node *left, Node *right);

    int get_freq() const;

    Node *left() const;

    Node *right() const;

    ~Node() = default;

private:
    Node *m_left;
    Node *m_right;

    int m_frequency;
};


class BinTree {
public:
    explicit BinTree(const std::vector<std::pair<uint8_t, int> > &list);

    ~BinTree() = default;

    Node *build_tree();


    void print_tree();

    void print_root(Node *root);

private:
    Node *m_root;
    std::vector<Node *> m_list;
};


#endif //BINTREE_H
