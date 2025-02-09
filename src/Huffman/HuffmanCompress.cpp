//

#include "../../include/Huffman/HuffmanCompress.h"

// Constructor
HuffmanCompress::HuffmanCompress(std::ifstream& inFile, std::ofstream& outFile, BitBuffer& bitBuffer, HuffmanDictionary& hd, HuffmanProperties& hp):
in_file(inFile),
out_file(outFile),
bb(bitBuffer) {
    m_extension = hp.get_extension();
    m_encoded = hd.get_encoded();
}




// Main function, compresses the file, using Huffman method
void HuffmanCompress::compress_file() {
    write_header();
    write_dictionary();
    write_encoded_data();
    update_header();
}


// Writes help information to the beginning of the compressed file (extension, dictionary amount etc.)
void HuffmanCompress::write_header() {

    uint8_t extension_size = m_extension.size();
    std::string extension = m_extension;

    out_file.write(reinterpret_cast<char *>(&m_bits_amount), sizeof(m_bits_amount));
    out_file.write(reinterpret_cast<char *>(&m_prep_size), sizeof(m_prep_size));
    out_file.write(reinterpret_cast<char *>(&m_dict_amount), sizeof(m_dict_amount));
    out_file.write(reinterpret_cast<char *>(&extension_size), sizeof(extension_size));

    for (size_t i = 0; i < extension_size; i++) {
        out_file.write(&(extension[i]), sizeof(char));
    }
}


// Fills up the dictionary for compressing and future decompressing
void HuffmanCompress::write_dictionary() {
    for (const auto &i : m_encoded) {
        uint8_t symbol = i.first;
        std::string encoded = i.second;
        uint8_t length = encoded.length();

        out_file.write(reinterpret_cast<char *>(&symbol), sizeof(symbol));
        out_file.write(reinterpret_cast<char *>(&length), sizeof(length));

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


// Function for writing an actual encoded message in the compressed file
void HuffmanCompress::write_encoded_data() {

    in_file.seekg(0, std::ios::beg);
    while (in_file.peek() != EOF) {
        uint8_t byte = in_file.get();
        auto encoded_value = m_encoded[byte];
        for (auto c : encoded_value) {
            bb.write_bit(c - '0');
            m_bits_amount++;
        }
    }
    while (bb.get_size() != 0) bb.write_bit(0);
}


// Updating important information (such as counted amount of bits) in the file header
void HuffmanCompress::update_header() {
    m_prep_size += sizeof(m_prep_size) + sizeof(m_bits_amount) + m_extension.size() + sizeof(uint8_t) + sizeof(m_dict_amount);

    out_file.seekp(0, std::ios::beg);
    out_file.write(reinterpret_cast<char *>(&m_bits_amount), sizeof(m_bits_amount));
    out_file.write(reinterpret_cast<char *>(&m_prep_size), sizeof(m_prep_size));
    out_file.write(reinterpret_cast<char *>(&m_dict_amount), sizeof(m_dict_amount));
}
