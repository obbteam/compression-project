#include "../include/BitBuffer.h"

BitBuffer::BitBuffer(std::ostream &ostream) : os(ostream) {
    buffer = count = 0;
}

void BitBuffer::flush() {
    os.put(buffer);
    os.flush();
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