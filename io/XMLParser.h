// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#ifndef VACCIN_SIMULATOR_XMLPARSER_H
#define VACCIN_SIMULATOR_XMLPARSER_H

#include <streambuf>
#include <string>

class Hub;
class JObject;

class XMLParser {
public:
    XMLParser(const char* file);
    void parseXML(Hub&);
    JObject* parse();

private:
    const char* fileName;
};


#endif //VACCIN_SIMULATOR_XMLPARSER_H
