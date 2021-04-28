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
     * REQUIRE(!fileName.empty(), "Filename can't be empty!");
     */
    JObject* parse(const std::string& fileName);
    /**
     * Parses a HUB element into a JObject representing a hub
     * @param hubXML: TiXmlElement*; HUB element
     * @return JObject*; JSON representation of a hub
     * REQUIRE(properlyInitialized(), "XMLParser object hasn't been initialized properly!");
     * REQUIRE(hubXML != NULL, "XML element corrupted");
     */
    JObject* parseHub(TiXmlElement* hubXML);
    /**
     * Parses a VACCIN element into a JObject representing a vaccine
     * @param vaccinXML: TiXmlElement*; VACCIN element
     * @return JObject*; JSON representation of a vaccine
     * REQUIRE(properlyInitialized(), "XMLParser object hasn't been initialized properly!");
     * REQUIRE(!elements_vaccin.empty(), "Vaccine elements can't be empty!");
     * REQUIRE(vaccinXML != NULL, "XML element corrupted");
     */
    JObject* parseVaccin(TiXmlElement* vaccinXML);
    /**
     * Parses a CENTRUM element into a JObject representing a center name
     * @param hubXML: TiXmlElement*; CENTRUM element
     * @return JObject*; JSON representation of a center name
     * REQUIRE(properlyInitialized(), "XMLParser object hasn't been initialized properly!");
     * REQUIRE(centraXML != NULL, "XML element corrupted");
     */
    JArray* parseHubCenters(TiXmlElement* centraXML);
    /**
     * Parses a VACCINATIECENTRUM element into a JObject representing a VaccinationCenter
     * @param hubXML: TiXmlElement*; VACCINATIECENTRUM element
     * @return JObject*; JSON representation of a VaccinationCenter
     * REQUIRE(properlyInitialized(), "XMLParser object hasn't been initialized properly!");
     * REQUIRE(vaccinactionCenterXML != NULL, "XML element corrupted");
     */
    JObject* parseCenter(TiXmlElement* vaccinationCenterXML);

private:
    // Initialization
    const XMLParser *initCheck;

    // Parsing objects
    std::vector<std::string> elements_centra;
    std::vector<std::string> elements_vaccin;
};


#endif //VACCIN_SIMULATOR_XMLPARSER_H
