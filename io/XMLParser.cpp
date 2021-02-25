//
// Created by robbe on 25/02/2021.
//

#include "XMLParser.h"
#include "../tinyxml/tinyxml.h"
using namespace std;

XMLParser::XMLParser(const std::string& file): fileName(file){
}

std::streambuf XMLParser::readAsStream() const {
    TiXmlDocument xml_document;
    if (!xml_document.LoadFile(fileName)) {
    }
}