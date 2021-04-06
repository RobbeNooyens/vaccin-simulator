// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
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
    /**
     * ENSURE(properlyInitialized(), "XMLParser object hasn't been initialized properly!");
     */
    XMLParser();
    bool properlyInitialized() const;

    /**
     * Parses an XML file and puts its contents in a JObject
     * @param fileName: name of the XML file to parse
     * @return JObject: representation of the XML file
     * REQUIRE(properlyInitialized(), "XMLParser object hasn't been initialized properly!");
     */
    JObject* parse(const std::string& fileName);

private:
    // Initialization
    const XMLParser *initCheck;
};


#endif //VACCIN_SIMULATOR_XMLPARSER_H
