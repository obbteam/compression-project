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
        for (const auto &file: parser.getInputFiles()) {
            std::ifstream opened_file(file);
            if (opened_file.is_open()) {
                if (parser.getCompressionMethod() == "--huffman") {
                    std::cout << "Huffman: " << std::endl;
                    auto object = Huffman(opened_file);
                    std::cout << "Dictionary: " << std::endl;
                    object.print_dict();
                    std::cout << "Sorted vector: " << std::endl;
                    object.print_sorted();
                    std::cout << "File size: " << object.get_size() << " bytes." << std::endl;
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

        std::cout << "Item type: " << parser.getFileFolder() << "\n";
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
