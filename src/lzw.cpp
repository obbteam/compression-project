//
// Created by User on 1/12/2025.
//
#include "../include/lzw.h"

lzw::lzw(const std::string &file): m_file(file, std::ios::binary) {
    if (!m_file.is_open()) {
        throw std::runtime_error("Input file stream is not open.");
    }
    m_filename = file;

    for (int i = 0; i < 256; i++)
    {
        std::string temp;
        temp += char(i);
        m_encode[temp] = i;
        m_decode[i] = temp;

    }

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

    if (!m_file) return;

    std::string cur, next;
    int code = 256;
    cur = (char)m_file.get();
    std::cout << "String\tOutput_Code\tAddition\n";

    // While the file is not empty, keep encoding the values
    while (m_file.peek() != EOF)
    {
        next = (char)m_file.get();

        if (m_encode.find(cur + next) != m_encode.end()) {
            cur += next;
        } else {

            int output_code = m_encode[cur];
            file.write(reinterpret_cast<const char *>(&output_code), sizeof(output_code));

            std::cout << cur << "\t" << m_encode[cur] << "\t\t"
                      << cur + next << "\t" << code << std::endl;
            m_encode[cur + next] = code++;
            cur = next;
            next = "";
        }
        next = "";
    }

    // Write the code for the final string
    if (!cur.empty()) {
        int output_code = m_encode[cur];
        file.write(reinterpret_cast<const char*>(&output_code), sizeof(output_code));
        std::cout << cur << "\t" << output_code << std::endl;
    }


    file.close();
}

void lzw::decompress() {

}
