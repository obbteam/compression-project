//
// Created by Марк on 02.02.2025.
//

#include "../../include/Huffman/HuffmanProperties.h"

// Constructor
HuffmanProperties::HuffmanProperties(const std::string &file) {
    m_filename = file;
    m_extension = m_filename.substr(m_filename.find_last_of('.') + 1);
}


// Function for getting the file extension
std::string HuffmanProperties::get_extension() const {
    return m_extension;
}

// Function for getting the filename
std::string HuffmanProperties::get_filename() const {
    return m_filename.substr(0, m_filename.find_last_of('.'));
}