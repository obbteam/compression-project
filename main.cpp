#include <iostream>
#include "include/Parser.h"



int main() {
    comp::Parser parser;

    try {
        std::cout << "Enter your command (e.g., 'comp file.txt -ch') or 'q' to quit: ";
        std::string commandLine;
        std::getline(std::cin, commandLine);

        while (commandLine != "q"){
            parser.parse(commandLine);

            std::cout << "Utility Name: comp\n";
            std::cout << "Input Files: ";
            for (const auto &file: parser.getInputFiles()) {
                std::cout << file << " ";
            }
            std::cout << "\nCompression Method: " << parser.getCompressionMethod() << "\n";
            std::cout << "Operation: " << parser.getOperation() << "\n";

            std::cout << "Enter another command or 'q' to quit: ";
            std::getline(std::cin, commandLine);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}