#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <filesystem>


#ifndef COMPRESSION_PROJECT_PARSER_H
#define COMPRESSION_PROJECT_PARSER_H

namespace comp {
    class Parser {
    private:
        std::string expectedUtilityName = "comp";
        std::vector<std::string> inputFiles;
        std::string compressionMethod;
        std::string operation;

        std::unordered_map<std::string, std::string> optionMap = {
                {"-h", "--huffman"},
                {"-l", "--lzw"},
                {"-f", "--full"},
                {"-c", "--compress"},
                {"-d", "--decompress"}
        };

    public:
        // Parses the command line arguments
        void parse(const std::string& commandLine);

        // Accessor for input files
        const std::vector<std::string>& getInputFiles() const;

        // Accessor for compression method
        const std::string& getCompressionMethod() const;

        // Accessor for utility name
        const std::string& getOperation() const;
    };
}
#endif //COMPRESSION_PROJECT_PARSER_H
