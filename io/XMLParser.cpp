//
// Created by robbe on 25/02/2021.
//

#include "XMLParser.h"
#include "../tinyxml/tinyxml.h"
#include <iostream>

XMLParser::XMLParser(const std::string& file): fileName(file){
}

std::streambuf XMLParser::readAsStream() const {
    TiXmlDocument xml_document;
    if (!xml_document.LoadFile(fileName)) {
        std::cerr << xml_document.ErrorDesc() << std::endl;
        throw std::runtime_error("Error while loading file");
    }
}