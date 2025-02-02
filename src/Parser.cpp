#include "../include/Parser.h"

namespace comp {

    // Function that takes a string and parses it
    void Parser::parse(const std::string& commandLine) {
        std::istringstream stream(commandLine);
        std::string current;

        std::vector<std::string> args;
        while (stream >> current) {
            args.push_back(current);
        }

        if (args.size() < 3) {
            throw std::invalid_argument("Usage: <file1> [file2 ...]/<directory> <compression options> <file/directory>");
        }

        bool methodFound = false;
        bool fileFolderFound = false;

        for (size_t i = 0; i < args.size(); ++i) {
            const std::string& arg = args[i];

            if (arg[0] == '-') {
                for (const auto& [shortOpt, longOpt] : optionMap) {
                    if (arg.find(shortOpt) != std::string::npos) {
                        if (shortOpt == "-h" || shortOpt == "-l") {
                            if (methodFound) {
                                throw std::invalid_argument("Multiple compression methods provided. Only one method is allowed.");
                            }
                            compressionMethod = optionMap[shortOpt];
                            methodFound = true;
                        } else if (shortOpt == "-f" || shortOpt == "-d") {
                            if (fileFolderFound) {
                                throw std::invalid_argument("The item either needs to be a file or a directory.");
                            }
                            is_dir = optionMap[shortOpt];
                            fileFolderFound = true;
                        }
                    }
                }
            } else {
                inputFiles.push_back(arg);
            }
        }

        if (inputFiles.empty()) {
            throw std::invalid_argument("No input files specified.");
        }
        if (compressionMethod.empty()) {
            throw std::invalid_argument("No compression method specified.");
        }
        if (is_dir.empty()) {
            throw std::invalid_argument("No indication of a file or a directory was provided.");
        }

        if (is_dir == "--directory" && inputFiles.size() > 1) {
            throw std::invalid_argument("Cannot compress more than one directory.");
        }
    }



    // Get functions
    const std::vector<std::string>& Parser::getInputFiles() const {
        return inputFiles;
    }

    const std::string& Parser::getCompressionMethod() const {
        return compressionMethod;
    }

    const std::string& Parser::getFileFolder() const {
        return is_dir;
    }
}