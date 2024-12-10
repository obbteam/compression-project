#include "../include/Parser.h"

namespace comp {
    void Parser::parse(const std::string& commandLine) {
        std::istringstream stream(commandLine);
        std::string current;

        std::vector<std::string> args;
        while (stream >> current) {
            args.push_back(current);
        }

        if (args.size() < 4) {
            throw std::invalid_argument("Usage: <file1> [file2 ...]/<directory> <compression options> <operation> <file/directory>");
        }

        bool methodFound = false;
        bool operationFound = false;
        bool fileFolderFound = false;

        for (size_t i = 0; i < args.size(); ++i) {
            const std::string& arg = args[i];

            if (arg[0] == '-') {
                for (const auto& [shortOpt, longOpt] : optionMap) {
                    if (arg.find(shortOpt) != std::string::npos) {
                        if (shortOpt == "-h" || shortOpt == "-l" || shortOpt == "-f") {
                            if (methodFound) {
                                throw std::invalid_argument("Multiple compression methods provided. Only one method is allowed.");
                            }
                            compressionMethod = optionMap[shortOpt];
                            methodFound = true;
                        } else if (shortOpt == "-c" || shortOpt == "-d") {
                            if (operationFound) {
                                throw std::invalid_argument("Multiple operations provided. Only one operation is allowed.");
                            }
                            operation = optionMap[shortOpt];
                            operationFound = true;
                        } else if (shortOpt == "-F" || shortOpt == "-D") {
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
        if (operation.empty()) {
            throw std::invalid_argument("No operation specified (use -c or -d).");
        }
        if (is_dir.empty()) {
            throw std::invalid_argument("No indication of a file or a directory was provided.");
        }

        if (is_dir == "--directory" && inputFiles.size() > 1) {
            throw std::invalid_argument("Cannot compress more than one directory.");
        }
    }



    const std::vector<std::string>& Parser::getInputFiles() const {
        return inputFiles;
    }

    const std::string& Parser::getCompressionMethod() const {
        return compressionMethod;
    }

    const std::string& Parser::getOperation() const {
        return operation;
    }

    const std::string& Parser::getFileFolder() const {
        return is_dir;
    }
}