//
// Created by Марк on 02.02.2025.
//

#include "../../include/Huffman/HuffmanDictionary.h"

// Constructor
HuffmanDictionary::HuffmanDictionary(std::ifstream& file): in_file(file), m_size{0} {}



// Main function
void HuffmanDictionary::create_encoded_dictionary() {
    build_frequency_table();
    sort_frequency_table();
    build_huffman_tree();
}


// Function for building frequency table
void HuffmanDictionary::build_frequency_table() {
    while (in_file.peek() != EOF) {
        uint8_t byte;
        in_file.read(reinterpret_cast<char *>(&byte), sizeof(byte));
        ++m_size;
        m_dictionary[byte]++;
    }
}


// Function for sorting the frequency table
void HuffmanDictionary::sort_frequency_table() {
    m_sorted = std::vector<std::pair<uint8_t, int> >(m_dictionary.begin(), m_dictionary.end());
    std::sort(m_sorted.begin(), m_sorted.end(),
              [](const std::pair<uint8_t, int> &a, const std::pair<uint8_t, int> &b) {
                  return a.second < b.second;
              });
}


// Building huffman frequency tree
void HuffmanDictionary::build_huffman_tree() {
    auto object = BinTree(m_sorted);
    //object.print_tree();
    const auto root = object.get_root();
    encode_characters(root);
}


// Function for encoding all characters met in the file
void HuffmanDictionary::encode_characters(Node *root_node) {
    if (root_node == nullptr) return;

    create_code(root_node, 0, "");
}



// Function for recursively creating codes for each character in the file
void HuffmanDictionary::create_code(Node *root_node, uint8_t length, std::string &&code) {
    if (root_node == nullptr) return;

    if (length == 0 && root_node->left() == nullptr && root_node->right() == nullptr) {
        length++;
        code = "0";
    };

    if (root_node->get_data()) {
        // data is a character (uint8)
        auto data = root_node->get_data();
        auto it = m_dictionary.find(*data);
        if (it != m_dictionary.end()) {
            m_encoded.insert(std::make_pair(it->first, code));
        }
    }
    // concatenates 0 at the end
    create_code(root_node->left(), length + 1, code + '0');
    // concatenates 1 at the end
    create_code(root_node->right(), length + 1, code + '1');
}


// Print functions
void HuffmanDictionary::print_dict() const {
    std::cout << "\nDict: " << std::endl;
    for (const auto &[fst, snd]: m_dictionary) {
        std::cout << fst << ": " << snd << std::endl;
    }
}

void HuffmanDictionary::print_encoded() const {
    std::cout << "Encoded dict: " << std::endl;
    for (const auto &[fst, snd]: m_encoded) {
        std::cout << fst << ": " << snd << std::endl;
    }
}

void HuffmanDictionary::print_sorted() const {
    for (const auto &[fst, snd]: m_sorted) {
        std::cout << fst << ": " << snd << std::endl;
    }
}
