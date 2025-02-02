//
// Created by User on 12/15/2024.
//

#ifndef COMPRESSION_PROJECT_BITBUFFER_H
#define COMPRESSION_PROJECT_BITBUFFER_H

#include <ostream>
#include <istream>

class BitBuffer {
public:
    // Constructors
    explicit BitBuffer(std::ostream &ostream);
    explicit BitBuffer(std::istream &istream);

    // Main functions
    void flush();
    void clear();
    bool eof() const;
    int read_bit();
    void write_bit(int i);
    int get_size() const;


private:
    char buffer;
    int count;
    std::ostream* os = nullptr;

    std::istream* is = nullptr;
};

#endif //COMPRESSION_PROJECT_BITBUFFER_H
