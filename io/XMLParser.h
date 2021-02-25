//
// Created by robbe on 25/02/2021.
//

#ifndef VACCIN_SIMULATOR_XMLPARSER_H
#define VACCIN_SIMULATOR_XMLPARSER_H

#include <streambuf>
#include <string>

class XMLParser {
public:
    XMLParser(const std::string& file);
    std::streambuf readAsStream() const;

private:
    const std::string fileName;
};


#endif //VACCIN_SIMULATOR_XMLPARSER_H
