
#include "../../include/Huffman/Huffman.h"


// Constructor/destructor
Huffman::Huffman(const std::string &file): in_file(file, std::ios::binary), m_huffman_properties(file) {
    if (!in_file.is_open()) {
        throw std::runtime_error("Input file stream is not open.");
    }

}

Huffman::~Huffman() {
    in_file.close();
}




// Compress function
void Huffman::compress() {

    std::ofstream out_file(m_huffman_properties.get_filename() + ".groza", std::ios::binary);

    m_huffman_dict.create_encoded_dictionary();

    in_file.seekg(0, std::ios::beg);

    BitBuffer bb(out_file);

    auto huffmanCompress = HuffmanCompress(in_file, out_file, bb, m_huffman_dict, m_huffman_properties);
    huffmanCompress.compress_file();

    std::cout << "The prep size is " << huffmanCompress.getPrepSize() << std::endl;
    std::cout << "Bits amount in a new message is " << huffmanCompress.getBitsAmount() << std::endl;
    std::cout << "The amount of elements in the dictionary is " << int(huffmanCompress.getDictAmount() + 1) << std::endl;

    out_file.close();

}


// Decompress function
void Huffman::decompress() {
    in_file.seekg(0, std::ios::beg);

    auto huffmanDecompress = HuffmanDecompress(in_file);
    huffmanDecompress.read_header();
    std::string filename = m_huffman_properties.get_filename() + ".groza." + huffmanDecompress.getExtension();
    std::ofstream out_file(filename, std::ios::binary);
    if (!out_file.is_open()) {
        std::cerr << "\nError creating output file" << std::endl;
        return;
    }
    huffmanDecompress.read_dictionary();
    huffmanDecompress.decode_file(out_file);
    out_file.close();
}



// Print function
void Huffman::print_file() {
    int count = 0;
    while (in_file.peek() != EOF) {
        uint8_t byte = in_file.get();
        std::cout << std::bitset<8>(byte);

        count += 8;
        if (count % 8 == 0) {
            if (count % 64 == 0) {
                std::cout << std::endl;
            } else {
                std::cout << ' ';;
            }
        }
    }
}
