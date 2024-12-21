//
// Created by obbte on 18.12.2024.
//

#ifndef DECODE_H
#define DECODE_H
#include <fstream>
#include <unordered_map>

#endif //DECODE_H

class Decode {
public:
    explicit Decode(const std::string &file);

    ~Decode();

private:
    void populate_table();
    void read_encoded();
    std::ifstream m_file;
    uint64_t m_bits;
    uint16_t m_dict_size;
    std::unordered_map<uint16_t, uint8_t> m_decoded_table;
};
