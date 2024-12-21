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

        for (const auto &file: parser.getInputFiles()) {
            std::ifstream opened_file(file);
            if (opened_file.is_open()) {
                if (parser.getCompressionMethod() == "--huffman") {
                    std::cout << "Huffman: " << std::endl;
                    auto object = Huffman(opened_file);

                    std::string compFile_name = file.substr(0, file.find_last_of('.'));
                    if(parser.getOperation() == "--compress")
                    {
                        compFile_name += ".groza";
                        std::ofstream compFile(compFile_name);
                        object.compress(compFile);
                        compFile.close();
                    } else if (parser.getOperation() == "--decompress")
                    {
                        compFile_name += ".txt";
                        std::ofstream compFile(compFile_name);
                        object.decompress(compFile);
                        compFile.close();
                    }

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
