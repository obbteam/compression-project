//
// Created by obbte on 18.12.2024.
//

#include "../include/Decode.h"

#include <iostream>

Decode::Decode(const std::string &file): m_bits(0) {
    std::ifstream file_stream(file);
    if (file_stream.is_open()) {
        m_file = std::move(file_stream);
        uint8_t left = m_file.get();
        uint8_t right = m_file.get();
        m_dict_size = (left << 8) | right;

        for (int i = 0; i < 8; ++i) {
            m_bits = (m_bits << 8) | m_file.get();
        }

        populate_table();
        read_encoded();
    } else {
        std::cerr << "Error opening file" << std::endl;
    }
}

void Decode::populate_table() {
    for (int i = 0; i < (m_dict_size - 10) / 3; ++i) {
        // every element in a dict is 3 bytes:
        // 1 byte for a character
        //  2 bytes for encoded_size and encoded_message

        uint8_t character = m_file.get();
        uint8_t encoded_size = m_file.get();
        uint8_t encoded_message = m_file.get();
        m_decoded_table.emplace(encoded_size << 8 | encoded_message, character);
    }
}

Decode::~Decode() {
    m_file.close();
}

// reads bytes from m_file, parses them, looks for the correct
void Decode::read_encoded() {
    m_file.seekg(m_dict_size - 1, std::ios::beg);

    std::ofstream output_file("output.txt");

    if (!output_file.is_open()) std::cerr << "Error creating output file" << std::endl;

    // todo get the byte from m_file, check all the bits with the corresponding key in m_decoded_table
    // todo if we find the correct key then write the value into the output file
    // todo if not we need to store the leftover of the byte in order to join it with new bits from a new byte
    // note: the size of the key can be really huge (256 bits), also need to take it in mind

    while (m_file.peek() != EOF) {
        uint8_t byte = m_file.get();
        uint8_t buff = 0;

        for (uint8_t i = 1; i < 9; ++i) {
            // i is bits size (1-8)
            buff = buff << i | byte >> (8 - i);
            uint16_t key = i << 8 | buff;
            if (m_decoded_table.contains(key)) {
                output_file << m_decoded_table[key];
                // we need to remove the part that we checked already
                byte = byte >> i;
            }
        }
    }


    // дай бог тут все правильно
    output_file.close();
}
