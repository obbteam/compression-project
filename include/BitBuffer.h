//
// Created by User on 12/15/2024.
//

#ifndef COMPRESSION_PROJECT_BITBUFFER_H
#define COMPRESSION_PROJECT_BITBUFFER_H

#include <ostream>

class BitBuffer {
private:
    char buffer;
    int count;
    std::ostream& os;

public:
    explicit BitBuffer(std::ostream &ostream);
    void flush();
    int get_size() const;
    void write_bit(int i);
};

#endif //COMPRESSION_PROJECT_BITBUFFER_H
