#include <fstream>
#include <iostream>

#include "include/huffman.h"
#include "include/Parser.h"


int main(int argc, char *argv[]) {
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
                    file.decompress();
                } else {
                    file.compress();
                    file.print_encoded();
                }
            } else if (parser.getCompressionMethod() == "--lzw") {
                std::cout << "lzw" << std::endl;
            }

        }

        std::cout << "\nCompression Method: " << parser.getCompressionMethod() << "\n";

        std::cout << "Item type: " << parser.getFileFolder() << "\n";
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
