//
// Created by User on 12/15/2024.
//

#ifndef COMPRESSION_PROJECT_BITBUFFER_H
#define COMPRESSION_PROJECT_BITBUFFER_H

#include <ostream>
#include <istream>

class BitBuffer {
private:
    char buffer;
    int count;
    std::ostream* os = nullptr;
    std::istream* is = nullptr;

public:
    explicit BitBuffer(std::ostream &ostream);
    explicit BitBuffer(std::istream &istream);
    void flush();
    void clear();
    int get_size() const;
    void write_bit(int i);

    int read_bit();
    bool eof() const;
};

#endif //COMPRESSION_PROJECT_BITBUFFER_H
