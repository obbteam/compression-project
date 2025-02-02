//
// Created by Марк on 02.02.2025.
//

#include "../include/HuffmanCompress.h"


HuffmanCompress::HuffmanCompress(std::ifstream& in_file, std::ofstream& out_file, BitBuffer& bitBuffer, HuffmanDictionary& hd, HuffmanProperties& hp):
m_file(in_file),
file(out_file),
bb(bitBuffer) {
    m_extension = hp.get_extension();
    m_encoded = hd.get_encoded();
}


void HuffmanCompress::write_header() {

    uint8_t extension_size = m_extension.size();
    std::string extension = m_extension;

    file.write(reinterpret_cast<char *>(&m_bits_amount), sizeof(m_bits_amount));
    file.write(reinterpret_cast<char *>(&m_prep_size), sizeof(m_prep_size));
    file.write(reinterpret_cast<char *>(&m_dict_amount), sizeof(m_dict_amount));
    file.write(reinterpret_cast<char *>(&extension_size), sizeof(extension_size));

    for (size_t i = 0; i < extension_size; i++) {
        file.write(&(extension[i]), sizeof(char));
    }
}


void HuffmanCompress::write_dictionary() {
    for (const auto &i : m_encoded) {
        uint8_t symbol = i.first;
        std::string encoded = i.second;
        uint8_t length = encoded.length();

        file.write(reinterpret_cast<char *>(&symbol), sizeof(symbol));
        file.write(reinterpret_cast<char *>(&length), sizeof(length));

        for (auto c : encoded) {
            if (c != '\0') bb.write_bit(c - '0');
        }
        while (bb.get_size() != 0) {
            bb.write_bit(0);
        }

        auto encoded_bytes = length % 8 == 0 ? length / 8 : length / 8 + 1;
        m_prep_size += 2 + encoded_bytes;
    }
    m_dict_amount = m_encoded.size() - 1;
}


void HuffmanCompress::write_encoded_data() {

    m_file.seekg(0, std::ios::beg);
    while (m_file.peek() != EOF) {
        uint8_t byte = m_file.get();
        auto encoded_value = m_encoded[byte];
        for (auto c : encoded_value) {
            bb.write_bit(c - '0');
            m_bits_amount++;
        }
    }
    while (bb.get_size() != 0) bb.write_bit(0);
    std::cout << "Finished" << std::endl;
}


void HuffmanCompress::update_header() {
    m_prep_size += sizeof(m_prep_size) + sizeof(m_bits_amount) + m_extension.size() + sizeof(uint8_t) + sizeof(m_dict_amount);

    file.seekp(0, std::ios::beg);
    file.write(reinterpret_cast<char *>(&m_bits_amount), sizeof(m_bits_amount));
    file.write(reinterpret_cast<char *>(&m_prep_size), sizeof(m_prep_size));
    file.write(reinterpret_cast<char *>(&m_dict_amount), sizeof(m_dict_amount));
}

void HuffmanCompress::compress_file() {
    write_header();
    write_dictionary();
    write_encoded_data();
    update_header();
}