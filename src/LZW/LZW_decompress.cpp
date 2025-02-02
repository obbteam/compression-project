//
// Created by Марк on 02.02.2025.
//

#include "../../include/LZW/LZW_decompress.h"

// Constructor
LZW_decompress::LZW_decompress(std::ifstream &inFile): in_file(inFile) {}




// Reads the extension from the compressed input file
std::string LZW_decompress::read_extension() {
    uint8_t extension_size;
    in_file.read(reinterpret_cast<char *>(&extension_size), sizeof(extension_size));

    std::string extension(extension_size, '\0');
    in_file.read(&extension[0], extension_size);
    return extension;
}


// Reads 12 bit code from the compressed file, using BitBuffer
int LZW_decompress::read_code(BitBuffer &bitBuffer) {
    int code = 0;
    for (int i = 0; i < 12; i++) {
        auto bit = bitBuffer.read_bit();
        if (bit == -1) {
            std::cout << "Decompression done";
            return -1;
        }
        code = (code << 1) | bit;
    }
    return code;
}


// Writes data to the decompressed output file
void LZW_decompress::write_to_file(std::ofstream& out_file, const std::string &data) {
    out_file.write(data.data(), data.size());
}


// Goes through the whole input file, decompresses information and writes it to the output file
void LZW_decompress::process_compressed_data(BitBuffer &bitBuffer, std::string &cur, int &cur_code, int &dict_size, std::ofstream& out_file) {
    while (!bitBuffer.eof()) {
        int next_code = read_code(bitBuffer);
        if (next_code == -1) return;

        std::string decoded = get_decoded_string(cur_code, next_code);
        write_to_file(out_file, decoded);

        if (dict_size >= MAX_DICT_SIZE) {
            reset_dictionary(dict_size);
        }

        m_decoded[dict_size++] = cur + decoded[0];
        cur = decoded;
        cur_code = next_code;
    }
}


// Gets the string of the code depending on the next value in the stream
std::string LZW_decompress::get_decoded_string(int cur_code, int next_code) {
    if (m_decoded.contains(next_code)) {
        return m_decoded[next_code];
    }
    return m_decoded[cur_code] + m_decoded[cur_code][0];
}


// Resets the m_decoded dictionary when it is full
void LZW_decompress::reset_dictionary(int &dict_size) {
    m_decoded.clear();
    dict_size = 256;
    fill_decoded();
}


// Fills the m_decoded dictionary with basic characters when it's empty
void LZW_decompress::fill_decoded() {
    m_decoded.clear();
    for (int i = 0; i < 256; ++i) {
        std::string temp;
        temp += static_cast<char>(i);
        m_decoded[i] = temp;
    }
}
