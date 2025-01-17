#include "../include/BitBuffer.h"

#include <iostream>
// todo check if its okay to put the buffer to 0 here
BitBuffer::BitBuffer(std::ostream &ostream) : os(&ostream) {
    buffer = count = 0;
}

// upd: putting buffer to the first byte of inout stream,
// otherwise it was creating a magical 0x00 byte
BitBuffer::BitBuffer(std::istream &istream) : is(&istream), count{0} {
    buffer = is->get();
}

void BitBuffer::flush() {
    os->put(buffer);
    os->flush();
    buffer = count = 0;
}

void BitBuffer::write_bit(int i) {
    if (i != 0 && i != 1) {
        throw std::invalid_argument("Only 0 or 1 can be written as bits.");
    }

    buffer = (buffer << 1) | (i & 1);
    count++;

    if (count == 8) flush();
}

int BitBuffer::get_size() const {
    return count;
}


int BitBuffer::read_bit() {
    if (is == nullptr) {
        throw std::logic_error("BitBuffer is not in read mode.");
    }

    if (count == 8) {
        // If all bits in the buffer have been read, load the next byte
        buffer = is->get();
        if (is->eof()) {
            return -1;
        }
        count = 0;
    }

    int bit = (buffer >> (7 - count)) & 1; // Extract the next bit
    count++;
    return bit;
}


bool BitBuffer::eof() const {
    return is && is->eof() && count == 8;
}
