//
// Created by Марк on 02.02.2025.
//

#ifndef COMPRESSION_PROJECT_HUFFMANPROPERTIES_H
#define COMPRESSION_PROJECT_HUFFMANPROPERTIES_H

#include "string"
#include "fstream"
#include "iostream"

class HuffmanProperties {

public:
    // Constructor
    HuffmanProperties(const std::string &file);


    // Get functions
    std::string get_extension() const;
    std::string get_filename() const;

private:
    std::string m_filename;
    std::string m_extension;
};


#endif //COMPRESSION_PROJECT_HUFFMANPROPERTIES_H
