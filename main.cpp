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

        std::cout << "Utility Name: comp\n";
        std::cout << "Input Files: ";
        for (const auto &file: parser.getInputFiles()) {
            std::ifstream opened_file(file);
            if (opened_file.is_open()) {
                if (parser.getCompressionMethod() == "--huffman") {
                    auto object = Huffman(opened_file);
                    object.print_dict();
                } else if (parser.getCompressionMethod() == "--lzw") {
                    std::cout << "lzw" << std::endl;
                }
            } else {
                std::cout << "Error opening file " << file << "\n";
            }

            opened_file.close();
        }

        std::cout << "\nCompression Method: " << parser.getCompressionMethod() << "\n";
        std::cout << "Operation: " << parser.getOperation() << "\n";
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
