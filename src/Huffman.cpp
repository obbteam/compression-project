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
    // Calculating the size of the dictionary + the number of the size
    // 1 byte for the char, 2 bytes for the size/encoded, 1 byte for the space (+ 2 at the end for the uint16_t for the size)
    std::cout << "\nThe amount of elements in the dictionary is " << (m_encoded.size());

    // Step 1: Calculate the extension size and retrieve the extension
    uint8_t extension_size = get_extension().size();
    std::string extension = get_extension();

    // Adding additional space for the size of dictionary and amount of bits
    // to be able to separate dictionary and message parts in the file when decompressing
    m_prep_size += sizeof(m_prep_size) + sizeof(m_bits_amount) + extension_size + sizeof(extension_size) + sizeof(
        m_dict_amount);

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
    }


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
    file.seekp(0, std::ios::beg);
    file.write(reinterpret_cast<char *>(&m_bits_amount), sizeof(m_bits_amount));

    // Testing to check if 16 bits for the dict and 64 bits for the new size are correct
    std::cout << "\nThe prep size is " << m_prep_size << std::endl;
    std::cout << "\nThe new size is " << m_bits_amount << std::endl;

    file.close();
}


void Huffman::decompress() {
    m_file.read(reinterpret_cast<char *>(&m_bits_amount), sizeof(m_bits_amount));
    m_file.read(reinterpret_cast<char *>(&m_prep_size), sizeof(m_prep_size));
    m_file.read(reinterpret_cast<char *>(&m_dict_amount), sizeof(m_dict_amount));

    // Step 1: Read the extension size
    uint8_t extension_size;
    m_file.read(reinterpret_cast<char *>(&extension_size), sizeof(extension_size));


    std::string extension;
    extension.resize(int(extension_size));
    m_file.read(&extension[0], extension_size);


    std::string filename = get_filename() + ".groza." + extension;
    std::cout << "\nDecompress: Decompressed filename: " << filename << '\n';

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) std::cerr << "\nError creating output file" << std::endl;


    m_decoded.clear();

    auto bb = BitBuffer(m_file);

    for (int i = 0; i < m_dict_amount; ++i) {
        uint8_t character, encoded_size;
        std::string encoded_message = "";
        m_file.read(reinterpret_cast<char *>(&character), sizeof(character));
        m_file.read(reinterpret_cast<char *>(&encoded_size), sizeof(encoded_size));
        encoded_message.reserve(encoded_size);


        // Correct way to round up to the nearest multiple of 8:
        int bits_to_read = ((encoded_size + 7) / 8) * 8;
        // or: int bits_to_read = (encoded_size + 7) & ~7;

        // Now read 'bits_to_read' bits from bb to stay aligned
        for (int j = 0; j < bits_to_read; ++j) {
            int bit = bb.read_bit();
            // Only the first 'encoded_size' bits are meaningful for the code
            if (j < encoded_size) {
                encoded_message.push_back('0' + bit);  // Convert bit (0 or 1) to char ('0' or '1')
            }
        }


        m_decoded[encoded_message] = character;
    }


    std::cout << "Dictionary entries:\n";
    for (const auto &[fst,snd]: m_decoded) {
        std::cout << " char=" << fst << " code=" << snd << std::endl;
    }


    m_file.seekg(m_prep_size, std::ios::beg);

    std::string code = "";
    uint64_t bits_read = 0;

    try {
        bb.flush();
        while (bits_read < m_bits_amount) {
            auto bit = bb.read_bit();
            bits_read++;

            code += std::to_string(bit);

            if (m_decoded.contains(code)) {
                file.write(reinterpret_cast<char *>(&m_decoded[code]), sizeof(m_decoded[code]));
                code = "";
            }
        }
    } catch (const std::exception &e) {
        std::cout << "End of file" << std::endl;
    }
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

    if (length == 0 && root_node->left() == nullptr && root_node->right() == nullptr) length++;

    if (root_node->get_data()) {
        // data is a character (uint8)
        auto data = root_node->get_data();
        auto it = m_dictionary.find(*data);
        if (it != m_dictionary.end()) {
            m_encoded.insert(std::make_pair(it->first, code));

            // 2 for 2 bytes of char and length of string
            // max 256 coz 256 ascii and 256 is theoretical max for depth of huffman tree
            // encoded_bytes is number of bytes needed to store an encoded value
            auto encoded_bytes = code.length() % 8 == 0 ? code.length() / 8 : code.length() / 8 + 1;
            m_prep_size += 2 + encoded_bytes;
            m_dict_amount++;
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
