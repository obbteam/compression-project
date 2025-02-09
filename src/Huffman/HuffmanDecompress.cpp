//
// Created by Марк on 02.02.2025.
//

#include "../../include/Huffman/HuffmanDecompress.h"

// Constructor
HuffmanDecompress::HuffmanDecompress(std::ifstream& inFile): in_file(inFile) {}


// Helper functions
void HuffmanDecompress::read_header() {
    in_file.read(reinterpret_cast<char *>(&m_bits_amount), sizeof(m_bits_amount));
    in_file.read(reinterpret_cast<char *>(&m_prep_size), sizeof(m_prep_size));
    in_file.read(reinterpret_cast<char *>(&m_dict_amount), sizeof(m_dict_amount));

    uint8_t extension_size;
    in_file.read(reinterpret_cast<char *>(&extension_size), sizeof(extension_size));
    m_extension.resize(int(extension_size));
    in_file.read(&m_extension[0], extension_size);

    /*
    std::cout << "\nBits amount: " << m_bits_amount << ", Prep size: " << m_prep_size
              << ", Dict amount: " << int(m_dict_amount) << ", Extension: " << m_extension << std::endl;
    */
}


// Reading dictionary from the file header for future message decoding
void HuffmanDecompress::read_dictionary() {
    in_file.seekg(sizeof(m_bits_amount) +
                 sizeof(m_prep_size) +
                 sizeof(m_dict_amount) +
                 sizeof(uint8_t) +
                 m_extension.size(),
                 std::ios::beg);

    m_decoded.clear();

    // 256 ascii characters didn't fit into 1 byte (uint8_t),
    // so we mapped the dict amount to start from 0
    // m_dict_amount -> 0 is actual amount of one element
    for (int i = 0; i <= m_dict_amount; ++i) {

        // --- 1) Read the symbol and length as raw bytes from the file
        uint8_t character{0};
        uint8_t encoded_size{0};
        in_file.read(reinterpret_cast<char *>(&character), sizeof(character));
        in_file.read(reinterpret_cast<char *>(&encoded_size), sizeof(encoded_size));

        // --- 2) Now read the Huffman code bits from the BitBuffer
        std::string encoded_message = read_huffman_code(encoded_size);
        // Store the mapping from bit-string -> character
        m_decoded[encoded_message] = character;

        /*
        std::cout << character << " of the size " << uint8_t(encoded_size) << " or " << int(encoded_size) << " " <<
                  encoded_message << std::endl;
        */
    }
}


// Reading the string from the file based on the code and size
std::string HuffmanDecompress::read_huffman_code(uint8_t encoded_size) {
    std::string encoded_message;
    encoded_message.reserve(encoded_size);
    auto bb = BitBuffer(in_file);

    for (int bitIndex = 0; bitIndex < encoded_size; ++bitIndex) {
        encoded_message.push_back(bb.read_bit() ? '1' : '0');
    }

    int mod = encoded_size % 8;
    if (mod != 0) {
        int leftover = 8 - mod;
        for (int p = 0; p < leftover; p++) {
            bb.read_bit(); // Discard leftover padding
        }
    }

    return encoded_message;
}


// Going through the whole file and decoding it based on the dictionary
void HuffmanDecompress::decode_file(std::ofstream &out_file) {
    std::string code;
    uint64_t bits_read = 0;
    auto bb = BitBuffer(in_file);

    try {
        while (bits_read < m_bits_amount) {
            int bit = bb.read_bit();
            if (bit == -1) break; // No more bits available

            bits_read++;
            code.push_back(bit ? '1' : '0');
            if (m_decoded.contains(code)) {
                out_file.write(reinterpret_cast<char *>(&m_decoded[code]), sizeof(m_decoded[code]));
                code.clear();
            }
        }
    } catch (const std::exception &e) {
        std::cout << "End of file" << std::endl;
    }
}