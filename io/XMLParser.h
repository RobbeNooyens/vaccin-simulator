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
#include <vector>

// TODO: Refactor to namespace

class Hub;
class JObject;
class JArray;

class TiXmlElement;

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
    JObject* parseHub(TiXmlElement* hubXML);
    JObject* parseVaccin(TiXmlElement* vaccinXML);
    JArray* parseHubCenters(TiXmlElement* centraXML);
    JObject* parseCenter(TiXmlElement* vaccinationCenterXML);

private:
    // Initialization
    const XMLParser *initCheck;

    // Parsing objects
    std::vector<std::string> elements_centra;
    std::vector<std::string> elements_vaccin;
};


#endif //VACCIN_SIMULATOR_XMLPARSER_H
