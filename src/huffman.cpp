//
// Created by obbte on 03.12.2024.
//
#include "../include/huffman.h"


// Constructor/deconstructor

Huffman::Huffman(const std::string &file): m_file(file), m_size{0} {
    if (!m_file.is_open()) {
        throw std::runtime_error("Input file stream is not open.");
    }
    m_filename = file;

}

Huffman::~Huffman() {
    m_file.seekg(0, std::ios::beg);
}


// Compress/decompress functions

void Huffman::compress() {
    // todo 1 - traverse binary tree to encode a byte
    // todo 2 - encode file in the .groza format
    // todo 3 - decoding using the dictionary in the file

    std::string filename = get_filename() + ".groza";
    std::ofstream file(filename, std::ios::binary);

    build_frequency_table();
    sort_frequency_table();
    build_huffman_tree();


    m_file.seekg(0, std::ios::beg);
    // Calculating the size of the dictionary + the number of the size
    // 1 byte for the char, 2 bytes for the size/encoded, 1 byte for the space (+ 2 at the end for the uint16_t for the size)
    std::cout << "\nThe amount of elements in the dictionary is " << (m_encoded.size());



    // Step 1: Calculate the extension size and retrieve the extension
    uint8_t extension_size = get_extension().size();
    std::string extension = get_extension();

    file.write(reinterpret_cast<char*>(&extension_size), sizeof(extension_size));

    for (size_t i = 0; i < extension_size; i++) {
        file.write(&(extension[i]), sizeof(char));
    }



    uint16_t dict_size = m_encoded.size() * 3 + 10 + int(extension_size) + 1;
    uint64_t bits_amount = 0;



    file.write(reinterpret_cast<char *>(&dict_size), sizeof(dict_size));
    file.write(reinterpret_cast<char *>(&bits_amount), sizeof(bits_amount));

    // todo check if write fails

    for (const auto &i: m_encoded) {
        uint8_t symbol = std::move(i.first);
        uint8_t encoded_size = std::move(i.second) >> 8;
        uint8_t encoded_code = std::move(i.second);


        file.write(reinterpret_cast<char *>(&symbol), sizeof(symbol));
        file.write(reinterpret_cast<char *>(&encoded_size), sizeof(encoded_size));
        file.write(reinterpret_cast<char *>(&encoded_code), sizeof(encoded_code));
    }


    // Reset beginning of the stream
    m_file.seekg(0, std::ios::beg);
    auto bitBuffer = BitBuffer(file);

    // For each character in the original file, write an encoded version to the compressed file
    while (m_file.peek() != EOF) {
        uint8_t byte = m_file.get();

        // Getting encoded version and its size
        uint16_t encoded_value = m_encoded[byte];
        int encoded_size = (encoded_value >> 8) & 0xFF;
        uint8_t encoded_byte = encoded_value & 0xFF;


        // Testing to see if the value, the byte and the size are correct
        //std::cout << "\nThe encoded value is " << std::bitset<16>(encoded_value) << ", the size is " << std::bitset<8>(encoded_size) << " and the encoded byte is " << std::bitset<8>(encoded_byte);


        // Adding encoded version bit by bit into the bitBuffer
        for (int i = encoded_size - 1; i >= 0; i--) {
            int bit = (encoded_byte >> i) & 0x1;
            bitBuffer.write_bit(bit);
            bits_amount++;
        }
    }

    while (bitBuffer.get_size() != 0) bitBuffer.write_bit(0);

    // Skipping the first 2 bytes that are the size of the dictionary, and updating the following 8 bytes
    // that are the amount of bits in the new message.
    file.seekp(3 + int(extension_size));
    file.write(reinterpret_cast<char *>(&bits_amount), sizeof(bits_amount));

    // Testing to check if 16 bits for the dict and 64 bits for the new size are correct
    std::cout << "\nThe size of dictionary is " << (dict_size) << " or " << std::bitset<16>(dict_size);
    std::cout << "\nThe new size is " << (bits_amount) << " or " << std::bitset<64>(bits_amount);

    file.close();
}


void Huffman::decompress() {

// Step 1: Read the extension size
    uint8_t extension_size;
    m_file.read(reinterpret_cast<char *>(&extension_size), sizeof(extension_size));


    std::string extension;
    extension.resize(int(extension_size));
    m_file.read(&extension[0], extension_size);


    std::string filename = get_filename() + "groza." + extension;
    std::cout << "Decompress: Decompressed filename: " << filename << '\n';

    std::ofstream file(filename, std::ios::binary);

    uint16_t dict_size;
    uint64_t bits_amount;
    m_file.read(reinterpret_cast<char *>(&dict_size), sizeof(dict_size));
    m_file.read(reinterpret_cast<char *>(&bits_amount), sizeof(bits_amount));

    m_decoded.clear();


    for (int i = 0; i < (dict_size - 10 - 1 - extension_size) / 3; ++i) {
        // every element in a dict is 3 bytes:
        // 1 byte for a character
        //  2 bytes for encoded_size and encoded_message

        uint8_t character = m_file.get();
        uint8_t encoded_size = m_file.get();
        uint8_t encoded_message = m_file.get();
        m_decoded[encoded_size << 8 | encoded_message] = character;
    }


    // std::cout << "Dictionary entries:\n";
    // for (auto &p: m_decoded) {
    //     auto key = p.first; // 16-bit: top 8 bits = size, low 8 bits = code
    //     auto ch = p.second; // the decoded character
    //     std::bitset<8> size = key >> 8;
    //     std::bitset<8> code = key & 0xff;
    //
    //     std::cout << " char=" << ch
    //             << " size=" << size
    //             << " code=" << code
    //             << " (hex key=" << std::hex << key << std::dec << ")\n";
    // }

    m_file.seekg(dict_size, std::ios::beg);
    std::cout << "Bytes after dict: " << std::endl;
    int k = 0;
    while (m_file.peek() != EOF) {
        ++k;
        std::cout << std::bitset<8>(m_file.get()) << std::endl;
    }
    std::cout << "\nThe amount of elements after the dictionary is " << k << std::endl;

    m_file.seekg(dict_size, std::ios::beg);


    if (!file.is_open()) std::cerr << "Error creating output file" << std::endl;

    uint16_t code = 0;
    int code_size = 0;
    int bits_read = 0;
    // вот тут вот случается беда с некоторыми
    auto bitBuffer = BitBuffer(m_file);
    std::cout << "Position after reading dict: " << m_file.tellg() << std::endl;

    std::cout << "Decompressed file: " << std::endl;
    try {
        while (bits_read < bits_amount) {
            auto bit = bitBuffer.read_bit();
            bits_read++;

            code = (code << 1) | bit;
            code_size++;
            uint16_t key = code_size << 8 | code;

            std::cout << "bits_read: " << bits_read
                    << " bit: " << bit
                    << " code: " << std::bitset<16>(code)
                    << " code_size: " << code_size
                    << " key: " << std::bitset<16>(key) << std::endl;


            if (m_decoded.find(key) != m_decoded.end()) {
                std::cout << "Match: " << m_decoded[key] << std::endl;
                file.put(m_decoded[key]);

                code_size = 0;
                code = 0;
            }
        }
    } catch (const std::exception &e) {
        std::cout << "End of file" << std::endl;
    }
}


// Helper functions

void Huffman::build_frequency_table() {
    while (m_file.peek() != EOF) {
        uint8_t byte = m_file.get();
        ++m_size;
        m_dictionary[byte]++;
    }
}

void Huffman::sort_frequency_table() {
    m_sorted = std::vector<std::pair<uint8_t, int> >(m_dictionary.begin(), m_dictionary.end());
    std::sort(m_sorted.begin(), m_sorted.end(),
              [](const std::pair<uint8_t, int> &a, const std::pair<uint8_t, int> &b) {
                  return a.second < b.second;
              });
}

void Huffman::build_huffman_tree() {
    auto object = BinTree(m_sorted);
    object.print_tree();
    auto root = object.get_root();
    encode_characters(root);
}

void Huffman::encode_characters(Node *root_node) {
    if (root_node == nullptr) return;

    create_code(root_node, 0, 0x00);
}


void Huffman::create_code(Node *root_node, uint8_t length, uint8_t code) {
    if (root_node == nullptr) return;

    if (length == 0 && root_node->left() == nullptr && root_node->right() == nullptr) length++;

    if (root_node->get_data()) {
        auto data = root_node->get_data();
        auto it = m_dictionary.find(*data);
        if (it != m_dictionary.end()) {
            m_encoded.insert(std::make_pair(it->first, (length << 8) | (code)));
        }
    }
    // shift to the left once (creates 1 zero at the end)
    create_code(root_node->left(), length + 1, code << 1);
    // shift to the left and create one at the end
    create_code(root_node->right(), length + 1, (code << 1) | 1);
}


// Printer/size functions
void Huffman::print_dict() const {
    for (auto &it: m_dictionary) {
        std::cout << it.first << ": " << it.second << std::endl;
    }
}

void Huffman::print_encoded() const {
    std::cout << "Encoded dict: " << std::endl;
    for (auto &it: m_encoded) {
        std::cout << it.first << ": " << std::bitset<16>(it.second) << std::endl;
        std::cout << "   size-" << (it.second >> 8) << "   code-" << (it.second & 0x00FF) << std::endl;
    }
}

void Huffman::print_sorted() const {
    for (auto &it: m_sorted) {
        std::cout << it.first << ": " << it.second << std::endl;
    }
}

int Huffman::get_size() const {
    return m_size;
}

std::string Huffman::get_extension() {
    return m_filename.substr(m_filename.find_last_of('.') + 1);
}

std::string Huffman::get_filename() {
    return m_filename.substr(0, m_filename.find_last_of('.'));
}
