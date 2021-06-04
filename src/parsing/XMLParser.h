// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#ifndef VACCIN_SIMULATOR_XMLPARSER_H
#define VACCIN_SIMULATOR_XMLPARSER_H

#include <streambuf>
#include <string>
#include <vector>

class Hub;
class JObject;
class JArray;

class TiXmlElement;

enum ParseError{NO_ROOT, INCONSISTENT_SIMULATION, INVALID_TYPE, UNKNOWN_ELEMENT, EMPTY_ELEMENT, MISSING_ELEMENT, DUPLICATED_ELEMENT, FILE_NOT_FOUND};
typedef std::vector<ParseError> ParseErrors;

namespace XMLParser {

    /**
     * Parses an XML file and puts its contents valid a JObject
     * @param fileName&: string; name of the XML file to parse
     * @param errorStream: ostream&; stream to send error messages to
     * @param errors: vector<ParseError>; vector to push error enum values to
     * @return JObject: representation of the XML file
     * * REQUIRE(properlyInitialized(), "XMLParser object hasn't been initialized properly!");
     * * REQUIRE(!fileName.empty(), "Filename can't be empty!");
     */
    JObject *parse(const std::string &fileName, std::ostream &errorStream, ParseErrors &errors);

    /**
     * Parses a HUB element into a JObject representing a hub
     * @param hubXML: TiXmlElement*; HUB element
     * @param errorStream: ostream&; stream to send error messages to
     * @param errors: vector<ParseError>; vector to push error enum values to
     * @return JObject*; JSON representation of a hub
     * * REQUIRE(properlyInitialized(), "XMLParser object hasn't been initialized properly!");
     * * REQUIRE(hubXML != NULL, "XML element corrupted");
     */
    JObject *parseHub(TiXmlElement *hubXML, std::ostream &errorStream, ParseErrors &errors);

    /**
     * Parses a VACCIN element into a JObject representing a vaccine
     * @param vaccinXML: TiXmlElement*; VACCIN element
     * @param errorStream: ostream&; stream to send error messages to
     * @param errors: vector<ParseError>; vector to push error enum values to
     * @return JObject*; JSON representation of a vaccine
     * * REQUIRE(properlyInitialized(), "XMLParser object hasn't been initialized properly!");
     * * REQUIRE(!elements_vaccin.empty(), "Vaccine elements can't be empty!");
     * * REQUIRE(vaccinXML != NULL, "XML element corrupted");
     */
    JObject *parseVaccin(TiXmlElement *vaccinXML, std::ostream &errorStream, ParseErrors &errors);

    /**
     * Parses a CENTRUM element into a JObject representing a center name
     * @param hubXML: TiXmlElement*; CENTRUM element
     * @param errorStream: ostream&; stream to send error messages to
     * @param errors: vector<ParseError>; vector to push error enum values to
     * @return JObject*; JSON representation of a center name
     * * REQUIRE(properlyInitialized(), "XMLParser object hasn't been initialized properly!");
     * * REQUIRE(centraXML != NULL, "XML element corrupted");
     */
    JArray *parseHubCenters(TiXmlElement *centraXML, std::ostream &errorStream, ParseErrors &errors);

    /**
     * Parses a VACCINATIECENTRUM element into a JObject representing a VaccinationCenter
     * @param hubXML: TiXmlElement*; VACCINATIECENTRUM element
     * @param errorStream: ostream&; stream to send error messages to
     * @param errors: vector<ParseError>; vector to push error enum values to
     * @return JObject*; JSON representation of a VaccinationCenter
     * * REQUIRE(properlyInitialized(), "XMLParser object hasn't been initialized properly!");
     * * REQUIRE(vaccinactionCenterXML != NULL, "XML element corrupted");
     */
    JObject *parseCenter(TiXmlElement *vaccinationCenterXML, std::ostream &errorStream, ParseErrors &errors);
}


#endif //VACCIN_SIMULATOR_XMLPARSER_H
