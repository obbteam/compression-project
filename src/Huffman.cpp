//
// Created by obbte on 03.12.2024.
//
#include "../include/Huffman.h"


// Constructor/destructor
Huffman::Huffman(const std::string &file): m_file(file, std::ios::binary), m_size{0} {
    if (!m_file.is_open()) {
        throw std::runtime_error("Input file stream is not open.");
    }
    m_filename = file;
}

Huffman::~Huffman() {
    m_file.close();
}


// Compress/decompress functions
void Huffman::compress() {
    // 1 - traverse binary tree to encode a byte
    // 2 - encode file in the .groza format
    // 3 - decoding using the dictionary in the file

    std::string filename = get_filename() + ".groza";
    std::ofstream file(filename, std::ios::binary);

    build_frequency_table();
    sort_frequency_table();
    build_huffman_tree();


    m_file.seekg(0, std::ios::beg);

    // Step 1: Calculate the extension size and retrieve the extension
    uint8_t extension_size = get_extension().size();
    std::string extension = get_extension();

    file.write(reinterpret_cast<char *>(&m_bits_amount), sizeof(m_bits_amount));
    file.write(reinterpret_cast<char *>(&m_prep_size), sizeof(m_prep_size));
    file.write(reinterpret_cast<char *>(&m_dict_amount), sizeof(m_dict_amount));

    file.write(reinterpret_cast<char *>(&extension_size), sizeof(extension_size));

    for (size_t i = 0; i < extension_size; i++) {
        file.write(&(extension[i]), sizeof(char));
    }


    auto bb = BitBuffer(file);
    for (const auto &i: m_encoded) {
        uint8_t symbol = i.first;
        std::string encoded = i.second;
        uint8_t length = encoded.length();

        file.write(reinterpret_cast<char *>(&symbol), sizeof(symbol));
        file.write(reinterpret_cast<char *>(&length), sizeof(length));

        for (auto c: encoded) {
            bb.write_bit(std::stoi(&c));
        }
        while (bb.get_size() != 0) {
            bb.write_bit(0);
        }

        // 2 for 2 bytes of char and length of string
        // max 256 coz 256 ascii and 256 is theoretical max for depth of huffman tree
        // encoded_bytes is number of bytes needed to store an encoded value
        auto encoded_bytes = length % 8 == 0 ? length / 8 : length / 8 + 1;
        m_prep_size += 2 + encoded_bytes;
    }
    m_dict_amount = m_encoded.size() - 1;


    // Reset beginning of the stream
    m_file.seekg(0, std::ios::beg);

    // // For each character in the original file write an encoded version to the compressed file
    while (m_file.peek() != EOF) {
        uint8_t byte = m_file.get();

        // Getting encoded version and its size
        auto encoded_value = m_encoded[byte];
        // Adding encoded version bit by bit into the bitBuffer
        for (auto c: encoded_value) {
            bb.write_bit(std::stoi(&c));
            m_bits_amount++;
        }
    }
    while (bb.get_size() != 0) bb.write_bit(0);

    // Skipping the first 2 bytes that are the size of the dictionary, and updating the following 8 bytes
    // that are the amount of bits in the new message.
    //todo check

    // Adding additional space for the size of dictionary and amount of bits
    // to be able to separate dictionary and message parts in the file when decompressing
    m_prep_size += sizeof(m_prep_size) + sizeof(m_bits_amount) + extension_size + sizeof(extension_size) + sizeof(
        m_dict_amount);

    file.seekp(0, std::ios::beg);
    file.write(reinterpret_cast<char *>(&m_bits_amount), sizeof(m_bits_amount));
    file.write(reinterpret_cast<char *>(&m_prep_size), sizeof(m_prep_size));
    file.write(reinterpret_cast<char *>(&m_dict_amount), sizeof(m_dict_amount));


    // Testing to check if 16 bits for the dict and 64 bits for the new size are correct
    std::cout << "The prep size is " << m_prep_size << std::endl;
    std::cout << "Bits amount in a new message is " << m_bits_amount << std::endl;

    // Calculating the size of the dictionary + the number of the size
    // 1 byte for the char, 2 bytes for the size/encoded, 1 byte for the space (+ 2 at the end for the uint16_t for the size)

    // Adding + 1 for the m_dict_amount, because there are maximum possible amount of 256 elements, which would fit in the
    // uint8_t, but we need to start from 0x00, so that 256th element is 0xFF. If we started from 0x01, we then would have
    // to use unit16_t just for the case in which the m_dict_amount is 2


    std::cout << "The amount of elements in the dictionary is " << int(m_dict_amount + 1) << std::endl;


    file.close();
}

void Huffman::print_file() {
    int count = 0;
    while (m_file.peek() != EOF) {
        uint8_t byte = m_file.get();
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

void Huffman::decompress() {
    m_file.seekg(0, std::ios::beg);
    m_file.read(reinterpret_cast<char *>(&m_bits_amount), sizeof(m_bits_amount));
    m_file.read(reinterpret_cast<char *>(&m_prep_size), sizeof(m_prep_size));
    m_file.read(reinterpret_cast<char *>(&m_dict_amount), sizeof(m_dict_amount));

    // Step 1: Read the extension size
    uint8_t extension_size;
    m_file.read(reinterpret_cast<char *>(&extension_size), sizeof(extension_size));


    std::string extension;
    extension.resize(int(extension_size));
    m_file.read(&extension[0], extension_size);

    std::cout << "\nBits amount is " << m_bits_amount << ", the prep size is " << m_prep_size << ", the dict amount is "
            << int(m_dict_amount) << " and the extension is " << extension << std::endl;


    std::string filename = get_filename() + ".groza." + extension;
    std::cout << "\nDecompress: Decompressed filename: " << filename << '\n';

    std::ofstream out_file(filename, std::ios::binary);
    if (!out_file.is_open()) std::cerr << "\nError creating output file" << std::endl;


    // Dictionary reading

    m_file.seekg(sizeof(m_bits_amount) +
                 sizeof(m_prep_size) +
                 sizeof(m_dict_amount) +
                 sizeof(extension_size) +
                 extension_size,
                 std::ios::beg);

    m_decoded.clear();

    // 256 ascii characters didn't fit into 1 byte (uint8_t),
    // so we mapped the dict amount to start from 0
    // m_dict_amount -> 0 is actual amount of one element
    for (int i = 0; i <= m_dict_amount; ++i) {
        // --- 1) Read the symbol and length as raw bytes from the file
        uint8_t character{0};
        uint8_t encoded_size{0};

        m_file.read(reinterpret_cast<char *>(&character), sizeof(character));
        m_file.read(reinterpret_cast<char *>(&encoded_size), sizeof(encoded_size));


        // --- 2) Now read the Huffman code bits from the BitBuffer
        std::string encoded_message;
        encoded_message.reserve(encoded_size);

        auto bb = BitBuffer(m_file);

        for (int bitIndex = 0; bitIndex < encoded_size; ++bitIndex) {
            int bit = bb.read_bit(); // read one bit
            // std::cout << bit;
            encoded_message.push_back(bit ? '1' : '0');
        }
        // std::cout << std::endl;
        // Store the mapping from bit-string -> character
        m_decoded[encoded_message] = character;

        // Because you padded each code to a byte boundary in compress(), you need to
        // skip the leftover bits within that last partial byte (if the code length wasn't
        // a multiple of 8). However, since you did:
        //
        //    while (bb.get_size() != 0) bb.write_bit(0);
        //
        // in compress(), each code was padded out to a full byte. So we must "consume"
        // those leftover padding bits from the BitBuffer:
        int mod = encoded_size % 8;
        if (mod != 0) {
            int leftover = 8 - mod;
            for (int p = 0; p < leftover; p++) {
                bb.read_bit(); // discard leftover padding
            }
        }

        std::cout << character << " of the size " << uint8_t(encoded_size) << " or " << int(encoded_size) << " " <<
                encoded_message << std::endl;
    }

    // Now we have read all the dictionary entries properly
    // Next, you can read the actual data bits to decode the file:
    std::string code;
    uint64_t bits_read = 0;

    try {
        auto bb = BitBuffer(m_file);
        while (bits_read < m_bits_amount) {
            int bit = bb.read_bit();
            if (bit == -1) {
                // no more bits available
                break;
            }
            bits_read++;
            code.push_back(bit ? '1' : '0');

            // Check if we have a complete code
            if (m_decoded.contains(code)) {
                out_file.write(reinterpret_cast<char *>(&m_decoded[code]), sizeof(m_decoded[code]));
                code.clear();
            }
        }
    } catch (const std::exception &e) {
        std::cout << "End of file" << std::endl;
    }

    out_file.close();
}


// Helper functions
void Huffman::build_frequency_table() {
    while (m_file.peek() != EOF) {
        uint8_t byte;
        m_file.read(reinterpret_cast<char *>(&byte), sizeof(byte));
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
    const auto root = object.get_root();
    encode_characters(root);
}

void Huffman::encode_characters(Node *root_node) {
    if (root_node == nullptr) return;

    create_code(root_node, 0, "");
}


void Huffman::create_code(Node *root_node, uint8_t length, std::string &&code) {
    if (root_node == nullptr) return;

    if (length == 0 && root_node->left() == nullptr && root_node->right() == nullptr) {
        length++;
        code = "0";
    };

    if (root_node->get_data()) {
        // data is a character (uint8)
        auto data = root_node->get_data();
        auto it = m_dictionary.find(*data);
        if (it != m_dictionary.end()) {
            m_encoded.insert(std::make_pair(it->first, code));
        }
    }
    // shift to the left once (creates 1 zero at the end)
    create_code(root_node->left(), length + 1, code + '0');
    // shift to the left and create one at the end
    create_code(root_node->right(), length + 1, code + '1');
}


// Printer/size functions
void Huffman::print_dict() const {
    std::cout << "\nDict: " << std::endl;
    for (const auto &[fst, snd]: m_dictionary) {
        std::cout << fst << ": " << snd << std::endl;
    }
}

void Huffman::print_encoded() const {
    std::cout << "Encoded dict: " << std::endl;
    for (const auto &[fst, snd]: m_encoded) {
        std::cout << fst << ": " << snd << std::endl;
    }
}

void Huffman::print_sorted() const {
    for (const auto &[fst, snd]: m_sorted) {
        std::cout << fst << ": " << snd << std::endl;
    }
}

int Huffman::get_size() const {
    return m_size;
}

std::string Huffman::get_extension() const {
    return m_filename.substr(m_filename.find_last_of('.') + 1);
}

std::string Huffman::get_filename() const {
    return m_filename.substr(0, m_filename.find_last_of('.'));
}
