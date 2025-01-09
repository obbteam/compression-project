#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>


#ifndef COMPRESSION_PROJECT_PARSER_H
#define COMPRESSION_PROJECT_PARSER_H

namespace comp {
    class Parser {
    private:
        std::vector<std::string> inputFiles;
        std::string compressionMethod;
        std::string is_dir;

        std::unordered_map<std::string, std::string> optionMap = {
            {"-h", "--huffman"},
            {"-l", "--lzw"},
            {"-f", "--file"},
            {"-d", "--directory"}
        };

    public:
        // Parses the command line arguments
        void parse(const std::string &commandLine);

        // Accessor for input files
        const std::vector<std::string> &getInputFiles() const;

        // Accessor for compression method
        const std::string &getCompressionMethod() const;

        // Accessor for is_dir
        const std::string &getFileFolder() const;
    };
}
#endif //COMPRESSION_PROJECT_PARSER_H
