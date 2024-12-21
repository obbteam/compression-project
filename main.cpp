#include <fstream>
#include <iostream>

#include "include/Decode.h"
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

        if (parser.getOperation() == "--decompress") {
            std::string file = parser.getInputFiles()[0];
            auto object = Decode(file);
        } else {
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

                        std::string compFile_name = file.substr(0, file.find_last_of('.')) + ".groza";
                        std::cout << '\n' << compFile_name << '\n';
                        std::ofstream compFile(compFile_name);


                        if (compFile) {
                            object.outputfile(compFile);
                            compFile.close();
                        } else {
                            std::cout << "Error creating output file " << file << "\n";
                        }
                    } else if (parser.getCompressionMethod() == "--lzw") {
                        std::cout << "lzw" << std::endl;
                    }
                } else {
                    std::cout << "Error opening file " << file << "\n";
                }


                opened_file.close();
            }
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
