//
// Created by User on 1/12/2025.
//
#include "../include/LZW.h"

lzw::lzw(const std::string &file): m_file(file, std::ios::binary) {
    if (!m_file.is_open()) {
        throw std::runtime_error("Input file stream is not open.");
    }
    m_filename = file;
}

lzw::~lzw() {
    m_file.close();
}

std::string lzw::get_extension() {
    return m_filename.substr(m_filename.find_last_of('.') + 1);
}

std::string lzw::get_filename() {
    return m_filename.substr(0, m_filename.find_last_of('.'));
}


void lzw::compress() {
    std::string filename = get_filename() + ".groza";
    std::ofstream file(filename, std::ios::binary);

    if (!m_file.is_open() || !file.is_open()) {
        std::cerr << "Error opening file " << filename << std::endl;
        return;
    }

    uint8_t extension_size = get_extension().size();
    std::string extension = get_extension();

    file.write(reinterpret_cast<char *>(&extension_size), sizeof(extension_size));
    file.write(extension.data(), extension_size);


    fill_encoded();
    std::string cur, next;
    uint16_t dict_size = 256;
    cur = static_cast<char>(m_file.get());
    std::cout << "String\tOutput_Code\tAddition\n";
    auto bitBuffer = BitBuffer(file);


    // While the file is not empty, keep encoding the values
    while (m_file.peek() != EOF) {
        next = static_cast<char>(m_file.get());

        if (m_encoded.contains(cur + next)) {
            cur += next;
        } else {
            auto output_code = m_encoded[cur];

            // Adding encoded version bit by bit into the bitBuffer
            for (int i = 11; i >= 0; i--) {
                int bit = (output_code >> i) & 0x1;
                bitBuffer.write_bit(bit);
            }


            // std::cout << cur << "\t" << m_encoded[cur] << "\t\t"
            //         << cur + next << "\t" << dict_size << std::endl;


            // resetting the dictionary to 256 if the dict is full
            if (dict_size >= MAX_DICT_SIZE) {
                m_encoded.clear();
                dict_size = 256;
                fill_encoded();
            }
            m_encoded[cur + next] = dict_size++;

            cur = next;
        }
    }

    // Write the code for the final string
    if (!cur.empty()) {
        auto output_code = m_encoded[cur];
        for (int i = 11; i >= 0; i--) {
            int bit = (output_code >> i) & 0x1;
            bitBuffer.write_bit(bit);
        }
    }

    while (bitBuffer.get_size() != 0) bitBuffer.write_bit(0);


    file.close();
}


void lzw::decompress() {
    uint8_t extension_size;
    m_file.read(reinterpret_cast<char *>(&extension_size), sizeof(extension_size));


    std::string extension;
    extension.resize(extension_size);
    m_file.read(&extension[0], extension_size);

    std::cout << "String\tOutput_Code\tAddition\n";

    std::string filename = get_filename() + ".groza." + extension;
    std::ofstream file(filename, std::ios::binary);

    if (!m_file.is_open() || !file.is_open()) {
        std::cerr << "Error opening file " << filename << std::endl;
        return;
    }

    auto bitBuffer = BitBuffer(m_file);
    // todo rename the variables
    fill_decoded();
    int cur_code = 0;

    for (int i = 0; i < 12; i++) {
        auto b = bitBuffer.read_bit();
        if (b == -1) {
            std::cout << "Decompression done";
            return;
        }
        cur_code = cur_code << 1 | b;
    }


    std::string cur = m_decoded[cur_code];

    file.write(cur.data(), cur.size());

    std::cout << cur << "\t" << cur_code << std::endl;

    int dict_size = 256;
    while (!bitBuffer.eof()) {
        int next_code = 0;
        for (int i = 0; i < 12; i++) {
            auto b = bitBuffer.read_bit();
            if (b == -1) {
                std::cout << "Decompression done";
                return;
            }
            next_code = next_code << 1 | b;
        }

        std::string decoded;
        if (m_decoded.contains(next_code)) {
            decoded = m_decoded[next_code];
        } else {
            decoded = m_decoded[cur_code] + m_decoded[cur_code][0];
        }

        // write in the file
        file.write(decoded.data(), decoded.size());

        // std::cout << cur << "\t" << cur_code << "\t\t"
        //         << (cur + decoded) << "\t" << dict_size << std::endl;

        if (dict_size >= MAX_DICT_SIZE) {
            m_decoded.clear();
            dict_size = 256;
            fill_decoded();
        }

        m_decoded[dict_size++] = cur + decoded[0];
        cur = decoded;
        cur_code = next_code;
    }

    file.close();
}


void lzw::fill_encoded() {
    m_encoded.clear();
    for (int i = 0; i < 256; ++i) {
        std::string temp;
        temp += static_cast<char>(i);
        m_encoded[temp] = (i);
    }
}

void lzw::fill_decoded() {
    m_decoded.clear();
    for (int i = 0; i < 256; ++i) {
        std::string temp;
        temp += static_cast<char>(i);
        m_decoded[i] = temp;
    }
}
