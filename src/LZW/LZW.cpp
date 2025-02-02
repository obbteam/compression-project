//
// Created by User on 1/12/2025.
//
#include "../../include/LZW/LZW.h"

// Constructor and deconstructor
lzw::lzw(const std::string &file):
m_filename(file),
m_file(file, std::ios::binary) {}

lzw::~lzw() {
    m_file.close();
}



// Get functions
std::string lzw::get_extension() {
    return m_filename.substr(m_filename.find_last_of('.') + 1);
}

std::string lzw::get_filename() {
    return m_filename.substr(0, m_filename.find_last_of('.'));
}



// Compresses the file, using LZW method
void lzw::compress() {

    auto filename = get_filename() + ".groza";
    std::ofstream out_file(filename, std::ios::binary);

    auto m_compress = LZW_compress(m_file, get_extension());

    validate_files(m_file, out_file);
    m_compress.write_file_metadata(out_file);

    auto bitBuffer = BitBuffer(out_file);

    m_compress.encode_data(bitBuffer);
    m_compress.finalize_encoding(bitBuffer);

    out_file.close();
}



// Decompresses file, using LZW method
void lzw::decompress() {
    auto m_decompress = LZW_decompress(m_file);

    std::string filename = get_filename() + ".groza." + m_decompress.read_extension();
    std::ofstream out_file(filename, std::ios::binary);
    validate_files(m_file, out_file);

    auto bitBuffer = BitBuffer(m_file);
    m_decompress.fill_decoded();

    int cur_code = m_decompress.read_code(bitBuffer);
    if (cur_code == -1) return;

    std::string cur = m_decompress.get_decoded()[cur_code];
    m_decompress.write_to_file(out_file, cur);

    int dict_size = 256;
    m_decompress.process_compressed_data(bitBuffer, cur, cur_code, dict_size, out_file);
}





// Checks if both input and output files are open, throws an error if not
void lzw::validate_files(std::ifstream& inFile, std::ofstream& outFile) {
    if (!inFile.is_open()) {
        std::cerr << "Error opening input file "  << std::endl;
    }
    if (!outFile.is_open()) {
        std::cerr << "Error opening output file "  << std::endl;
    }
}