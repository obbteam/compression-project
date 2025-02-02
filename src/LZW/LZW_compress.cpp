//
// Created by Марк on 02.02.2025.
//

#include <utility>

#include "../../include/LZW/LZW_compress.h"


// Constructor
LZW_compress::LZW_compress(std::ifstream& inFile, std::string  extension):
    in_file(inFile),
    m_extension(std::move(extension))
    {}




// Writes file extension metadata to the output file
void LZW_compress::write_file_metadata(std::ofstream& out_file) {
    uint8_t extension_size = m_extension.size();
    std::string extension = m_extension;

    out_file.write(reinterpret_cast<char *>(&extension_size), sizeof(extension_size));
    out_file.write(extension.data(), extension_size);
}

// Encodes the input data using LZW compression
void LZW_compress::encode_data(BitBuffer &bitBuffer) {
    fill_encoded();
    std::string cur, next;
    uint16_t dict_size = 256;
    cur = static_cast<char>(in_file.get());

    while (in_file.peek() != EOF) {
        next = static_cast<char>(in_file.get());

        if (m_encoded.contains(cur + next)) {
            cur += next;
        } else {
            write_code(bitBuffer, m_encoded[cur]);

            if (dict_size >= MAX_DICT_SIZE) reset_dictionary(dict_size);

            m_encoded[cur + next] = dict_size++;
            cur = next;
        }
    }

    if (!cur.empty()) write_code(bitBuffer, m_encoded[cur]);
}

// Writes an encoded value to the bit buffer
void LZW_compress::write_code(BitBuffer &bitBuffer, uint16_t output_code) {
    for (int i = 11; i >= 0; i--) {
        int bit = (output_code >> i) & 0x1;
        bitBuffer.write_bit(bit);
    }
}

// Resets the dictionary when it reaches maximum size
void LZW_compress::reset_dictionary(uint16_t &dict_size) {
    m_encoded.clear();
    dict_size = 256;
    fill_encoded();
}

// Ensures the last bits are flushed
void LZW_compress::finalize_encoding(BitBuffer &bitBuffer) {
    while (bitBuffer.get_size() != 0) bitBuffer.write_bit(0);
}

// Fills encoded dictionary when it's empty
void LZW_compress::fill_encoded() {
    m_encoded.clear();
    for (int i = 0; i < 256; ++i) {
        std::string temp;
        temp += static_cast<char>(i);
        m_encoded[temp] = (i);
    }
}
