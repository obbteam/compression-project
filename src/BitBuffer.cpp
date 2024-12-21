#include "../include/BitBuffer.h"

BitBuffer::BitBuffer(std::ostream &ostream) : os(&ostream) {
    buffer = count = 0;
}

BitBuffer::BitBuffer(std::istream &istream) : is(&istream) {
    buffer = count = 0;
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

    if(count == 8) flush();
}

int BitBuffer::get_size() const {
    return count;
}


int BitBuffer::read_bit() {
    if (is == nullptr) {
        throw std::logic_error("BitBuffer is not in read mode.");
    }

    if (count == 8) { // If all bits in the buffer have been read, load the next byte
        is->get(buffer);
        if (is->eof()) {
            throw std::runtime_error("End of file reached while reading bits.");
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