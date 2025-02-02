#include <iostream>

#include "include/Huffman.h"
#include "include/Parser.h"
#include "include/LZW.h"


void tester() {
    std::ofstream file("numbers.bin", std::ios::binary);
    for (int i = 1; i <= 20000000; i++) {
        file.write(reinterpret_cast<char *>(&i), sizeof(i));
    }
    file.close();
}

void test_bit_buffer() {
    std::ifstream file("files/1.png", std::ios::binary);
    auto reader = BitBuffer(file);
    std::ofstream out("files/1.copy.png", std::ios::binary);
    auto writer = BitBuffer(out);
    while (!reader.eof()) {
        int bit = reader.read_bit();
        writer.write_bit(bit);
    }
    writer.flush();
}


int main(int argc, char *argv[]) {
    tester();
    comp::Parser parser;
    try {
        std::string commandLine;
        for (size_t i = 1; i < argc; i++) {
            commandLine += argv[i];
            commandLine += " ";
        }

        std::cout << commandLine << std::endl;
        parser.parse(commandLine);

        for (const auto &file_name: parser.getInputFiles()) {
            if (parser.getCompressionMethod() == "--huffman") {
                std::cout << "Huffman: " << std::endl;
                auto file = Huffman(file_name);

                if (file.get_extension() == "groza") {
                    // file.print_file();
                    file.decompress();
                } else {
                    file.compress();
                }
            } else if (parser.getCompressionMethod() == "--lzw") {
                std::cout << "lzw" << std::endl;
                auto file = lzw(file_name);

                if (file.get_extension() == "groza") {
                    file.decompress();
                } else {
                    file.compress();
                }
            }
        }

        std::cout << "\nCompression Method: " << parser.getCompressionMethod() << "\n";

        std::cout << "Item type: " << parser.getFileFolder() << "\n";
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }


    // std::ifstream file("test.groza", std::ios::binary);
    // int value;
    // while (file.read(reinterpret_cast<char*>(&value), sizeof(value))) {
    //     std::cout << "Output Code: " << value << std::endl;
    // }
    // file.close();

    return 0;
}
