// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#include <iostream>
#include "XMLParser.h"
#include "../../tinyxml/tinyxml.h"
#include "../json/JObject.h"
#include "../json/JValue.h"
#include "../json/JArray.h"
#include "../json/JKeys.h"
#include "../../DesignByContract.h"
#include "../utilities/utils.h"
#include <vector>
#include <set>

#define XML_HUB "HUB"
#define XML_HUB_CENTERS "CENTRA"
#define XML_HUB_CENTER "centrum"
#define XML_HUB_DELIVERY "levering"
#define XML_HUB_INTERVAL "interval"
#define XML_HUB_TRANSPORTATION "transport"
#define XML_VACCINE "VACCIN"
#define XML_VACCINE_TYPE "type"
#define XML_VACCINE_DELIVERY "levering"
#define XML_VACCINE_INTERVAL "interval"
#define XML_VACCINE_TRANSPORTATION "transport"
#define XML_VACCINE_RENEWING "hernieuwing"
#define XML_VACCINE_TEMPERATURE "temperatuur"
#define XML_CENTER "VACCINATIECENTRUM"
#define XML_CENTER_NAME "naam"
#define XML_CENTER_ADDRESS "adres"
#define XML_CENTER_INHABITANTS "inwoners"
#define XML_CENTER_CAPACITY "capaciteit"

#define DEFAULT_VACCINE_PARAMETERS 3

#define ITERATE_ELEMENTS(parent, child) for(TiXmlElement* child = parent->FirstChildElement(); child; child = child->NextSiblingElement())
#define COMMA ,

JObject* XMLParser::parse(const std::string& fileName, std::ostream &errorStream, std::vector<ParseError> &errors) {
    REQUIRE(!fileName.empty(), "Filename can't be empty!");
    REQUIRE(errorStream.good(), "Errorstream should be valid to properly send error messages!");
    // Check if file exists
    std::ifstream file;
    file.open(fileName.c_str());
    if(file.bad()) {
        errorStream << "File not found! Filename: " << fileName << std::endl;
        errors.push_back(FILE_NOT_FOUND);
        return NULL;
    }
    file.close();
    // Load XML Document
    TiXmlDocument xml_document = TiXmlDocument();
    if (!xml_document.LoadFile(fileName.c_str())) {
        errorStream << xml_document.ErrorDesc() << std::endl;
        throw std::runtime_error(xml_document.ErrorDesc());
    }

    // Load ROOT
    TiXmlElement* root = xml_document.FirstChildElement("ROOT");
    if (!root || root->NoChildren()) {
        xml_document.Clear();
        errorStream << "Failed to load file: No valid root element." << std::endl;
        errors.push_back(NO_ROOT);
        return NULL;
    }

    JObject* json = new JObject;
    JArray* hubs = new JArray();
    JArray* centers = new JArray();

    ITERATE_ELEMENTS(root, element) {
        // ROOT elements should be tinyxml elements
        if(element->Type() != element->TINYXML_ELEMENT) {
            errors.push_back(INVALID_TYPE);
            errorStream << "ROOT can only contains tinyxml elements!" << std::endl;
            continue;
        }
        if(element->NoChildren()) {
            errors.push_back(EMPTY_ELEMENT);
            errorStream << "Encountered an empty element in ROOT!" << std::endl;
            continue;
        }
        std::string elementName = element->Value();
        if(elementName == XML_HUB) {
            hubs->insertValue(new JValue(parseHub(element, errorStream, errors)));
        } else if(elementName == XML_CENTER) {
            centers->insertValue(new JValue(parseCenter(element, errorStream, errors)));
        } else {
            errors.push_back(UNKNOWN_ELEMENT);
            errorStream << "Encountered an unknown element in ROOT!" << std::endl;
        }
    }

    if (hubs->getItems().empty()) {
        errorStream << "Missing element: " << XML_HUB << std::endl;
        errors.push_back(MISSING_ELEMENT);
    }
    if (centers->getItems().empty()) {
        errorStream << "Missing element: VACCINATIECENTRUM" << XML_CENTER << std::endl;
        errors.push_back(MISSING_ELEMENT);
    }


    json->insertValue(SIMULATION_HUBS, new JValue(hubs));
    json->insertValue(SIMULATION_CENTERS, new JValue(centers));

    if(!ParseUtil::isConsistent(json)) {
        errors.push_back(INCONSISTENT_SIMULATION);
        errorStream << "Loaded simulation is inconsistent!" << std::endl;
    }

    xml_document.Clear();
    return json;
}

JObject *XMLParser::parseHub(TiXmlElement *hubXML, std::ostream &errorStream, std::vector<ParseError> &errors) {
    REQUIRE(hubXML, "XML element corrupted");
    REQUIRE(!hubXML->NoChildren(), "HUB should have children!");

    JObject* hub = new JObject;
    JArray* vaccines = new JArray();
    JArray* centers = new JArray();

    std::map<std::string, unsigned int> defaultVaccine;
    std::vector<std::string> centerNames;

    ITERATE_ELEMENTS(hubXML, element) {
        std::string elementName = element->Value();
        if(element->NoChildren()) {
            if(elementName == XML_HUB_DELIVERY || elementName == XML_HUB_INTERVAL || elementName == XML_HUB_TRANSPORTATION)
                defaultVaccine[elementName] = 0;
            errors.push_back(EMPTY_ELEMENT);
            errorStream << "Emtpy element found in HUB!" << std::endl;
            continue;
        }
        if(element->FirstChild()->Type() == element->TINYXML_ELEMENT) {
            if(elementName == XML_HUB_CENTERS) {
                centers = parseHubCenters(element, errorStream, errors);
            } else if(elementName == XML_VACCINE) {
                vaccines->insertValue(new JValue(parseVaccin(element, errorStream, errors)));
            } else {
                errors.push_back(UNKNOWN_ELEMENT);
                errorStream << "Found an unknown element in HUB!" << std::endl;
            }

        } else if(element->FirstChild()->Type() == element->TINYXML_TEXT) {
            if(elementName == XML_HUB_DELIVERY || elementName == XML_HUB_INTERVAL || elementName == XML_HUB_TRANSPORTATION) {
                // Check for duplicated elements
                if(defaultVaccine.find(elementName) != defaultVaccine.end()) {
                    errors.push_back(DUPLICATED_ELEMENT);
                    errorStream << "Found a duplicated element in HUB!" << std::endl;
                    continue;
                }
                defaultVaccine[elementName] = 0;
                TiXmlText* textElement = element->FirstChild()->ToText();
                if(!textElement) {
                    errors.push_back(EMPTY_ELEMENT);
                    errorStream << "Found an empty element in HUB!" << std::endl;
                    continue;
                }
                std::string sValue = textElement->Value();
                unsigned int value;
                if(StringUtil::stringToUnsignedInt(sValue, value)) {
                    defaultVaccine[elementName] = value;
                } else {
                    errors.push_back(INVALID_TYPE);
                    errorStream << "Invalid element type in HUB!" << std::endl;
                }
            } else {
                errors.push_back(UNKNOWN_ELEMENT);
                errorStream << "Found an unknown element in HUB!" << std::endl;
            }

        } else {
            errors.push_back(INVALID_TYPE);
            errorStream << "HUB can only contain tinyxml elements and text!" << std::endl;
            continue;
        }
    }

    if(defaultVaccine.size() == DEFAULT_VACCINE_PARAMETERS) {
        JObject* vaccine = new JObject();
        vaccine->insertValue(VACCINE_DELIVERY, new JValue(defaultVaccine[XML_HUB_DELIVERY]));
        vaccine->insertValue(VACCINE_INTERVAL, new JValue(defaultVaccine[XML_HUB_INTERVAL]));
        vaccine->insertValue(VACCINE_TRANSPORTATION, new JValue(defaultVaccine[XML_HUB_TRANSPORTATION]));
        vaccine->insertValue(VACCINE_TYPE, new JValue("Standaard"));
        vaccines->insertValue(new JValue(vaccine));
    } else if(!defaultVaccine.empty()) {
        if(defaultVaccine.find(XML_HUB_DELIVERY) == defaultVaccine.end()) {
            errors.push_back(MISSING_ELEMENT);
            errorStream << "Missing element: ROOT > HUB > " << XML_HUB_DELIVERY << std::endl;
        }
        if(defaultVaccine.find(XML_HUB_INTERVAL) == defaultVaccine.end()) {
            errors.push_back(MISSING_ELEMENT);
            errorStream << "Missing element: ROOT > HUB > " << XML_HUB_INTERVAL << std::endl;
        }
        if(defaultVaccine.find(XML_HUB_TRANSPORTATION) == defaultVaccine.end()) {
            errors.push_back(MISSING_ELEMENT);
            errorStream << "Missing element: ROOT > HUB > " << XML_HUB_TRANSPORTATION << std::endl;
        }
    }

    hub->insertValue(HUB_VACCINES, new JValue(vaccines));
    hub->insertValue(HUB_CENTERS, new JValue(centers));

    return hub;
}

JObject *XMLParser::parseVaccin(TiXmlElement *vaccinXML, std::ostream &errorStream, std::vector<ParseError> &errors) {
    REQUIRE(vaccinXML, "XML element corrupted");
    REQUIRE(!vaccinXML->NoChildren(), "VACCIN should have children!");

    std::map<std::string, bool> parsedElements;
    parsedElements.insert(std::pair<std::string, bool>(XML_VACCINE_TYPE, false));
    parsedElements.insert(std::pair<std::string, bool>(XML_VACCINE_DELIVERY, false));
    parsedElements.insert(std::pair<std::string, bool>(XML_VACCINE_INTERVAL, false));
    parsedElements.insert(std::pair<std::string, bool>(XML_VACCINE_TRANSPORTATION, false));
    parsedElements.insert(std::pair<std::string, bool>(XML_VACCINE_RENEWING, false));
    parsedElements.insert(std::pair<std::string, bool>(XML_VACCINE_TEMPERATURE, false));

    JObject* vaccine = new JObject;

    ITERATE_ELEMENTS(vaccinXML, element) {
        std::string name = element->Value();
        if(name == XML_VACCINE_TYPE) {
            if(parsedElements[name]) {
                errors.push_back(DUPLICATED_ELEMENT);
                errorStream << "Found a duplicated element in VACCIN!" << std::endl;
                continue;
            }
            TiXmlText* textElement = element->FirstChild()->ToText();
            if(!textElement) {
                errors.push_back(EMPTY_ELEMENT);
                errorStream << "Found an empty element in VACCIN!" << std::endl;
                continue;
            }
            std::string type = textElement->Value();
            vaccine->insertValue(VACCINE_TYPE, new JValue(type));
            parsedElements[name] = true;
        } else if(name == XML_VACCINE_TEMPERATURE) {
            if(parsedElements[name]) {
                errors.push_back(DUPLICATED_ELEMENT);
                errorStream << "Found a duplicated element in VACCIN!" << std::endl;
                continue;
            }
            TiXmlText* textElement = element->FirstChild()->ToText();
            if(!textElement) {
                errors.push_back(EMPTY_ELEMENT);
                errorStream << "Found an empty element in VACCIN!" << std::endl;
                continue;
            }
            std::string sValue = textElement->Value();
            double temperature;
            if(!StringUtil::stringToDouble(sValue, temperature)) {
                errors.push_back(INVALID_TYPE);
                errorStream << "Invalid type!" << std::endl;
                continue;
            }
            vaccine->insertValue(VACCINE_TEMPERATURE, new JValue(temperature));
            parsedElements[name] = true;
        } else if(name == XML_VACCINE_DELIVERY || name == XML_VACCINE_INTERVAL || name == XML_VACCINE_TRANSPORTATION || name == XML_VACCINE_RENEWING) {
            if(parsedElements[name]) {
                errors.push_back(DUPLICATED_ELEMENT);
                errorStream << "Found a duplicated element in VACCIN!" << std::endl;
                continue;
            }
            if(element->NoChildren()) {
                errors.push_back(EMPTY_ELEMENT);
                errorStream << "Found an empty element in VACCIN!" << std::endl;
                continue;
            }
            TiXmlText* textElement = element->FirstChild()->ToText();
            if(!textElement) {
                errors.push_back(INVALID_TYPE);
                errorStream << "Element of wrong type found in VACCIN!" << std::endl;
                continue;
            }
            std::string sValue = textElement->Value();
            unsigned int value;
            if(!StringUtil::stringToUnsignedInt(sValue, value)) {
                errors.push_back(INVALID_TYPE);
                errorStream << "Element of wrong type found in a VACCINATIECENTRUM!" << std::endl;
                continue;
            }
            if(name == XML_VACCINE_DELIVERY) {
                vaccine->insertValue(VACCINE_DELIVERY, new JValue(value));
            } else if(name == XML_VACCINE_INTERVAL) {
                vaccine->insertValue(VACCINE_INTERVAL, new JValue(value));
            } else if(name == XML_VACCINE_TRANSPORTATION) {
                vaccine->insertValue(VACCINE_TRANSPORTATION, new JValue(value));
            } else if(name == XML_VACCINE_RENEWING) {
                vaccine->insertValue(VACCINE_RENEWING, new JValue(value));
            }
            parsedElements[name] = true;
        } else {
            errors.push_back(UNKNOWN_ELEMENT);
            errorStream << "Unknown element in VACCIN detected!" << std::endl;
        }
    }

    if(!parsedElements[XML_VACCINE_TYPE]) {
        errors.push_back(MISSING_ELEMENT);
        errorStream << "Element 'type' required for a VACCIN!" << std::endl;
    } else if(!parsedElements[XML_VACCINE_DELIVERY]) {
        errors.push_back(MISSING_ELEMENT);
        errorStream << "Element 'levering' required for a VACCIN!" << std::endl;
    } else if(!parsedElements[XML_VACCINE_INTERVAL]) {
        errors.push_back(MISSING_ELEMENT);
        errorStream << "Element 'interval' required for a VACCIN!" << std::endl;
    } else if(!parsedElements[XML_VACCINE_TRANSPORTATION]) {
        errors.push_back(MISSING_ELEMENT);
        errorStream << "Element 'transport' required for a VACCIN!" << std::endl;
    }

    return vaccine;
}

JArray *XMLParser::parseHubCenters(TiXmlElement *centraXML, std::ostream &errorStream, std::vector<ParseError> &errors) {
    REQUIRE(centraXML, "XML element corrupted");
    REQUIRE(!centraXML->NoChildren(), "HUB should have centers!");

    JArray* centers = new JArray();
    std::set<std::string> centerNames;

    ITERATE_ELEMENTS(centraXML, element) {
        std::string elementName = element->Value();
        if(elementName != XML_HUB_CENTER) {
            errors.push_back(UNKNOWN_ELEMENT);
            errorStream << "Unknown element in HUB's centra!" << std::endl;
            continue;
        }
        if(element->NoChildren()) {
            errors.push_back(EMPTY_ELEMENT);
            errorStream << "Empty element found in HUB's centra!" << std::endl;
            continue;
        }
        TiXmlNode* node = element->FirstChild();
        node->ToText();
        TiXmlText* textElement = element->FirstChild()->ToText();
        if(!textElement) {
            errors.push_back(INVALID_TYPE);
            errorStream << "Element of wrong type found in HUB's centra!" << std::endl;
            continue;
        }
        std::string centerName = textElement->Value();
        if(MAP_CONTAINS_KEY(centerNames, centerName)) {
            errors.push_back(DUPLICATED_ELEMENT);
            errorStream << "Duplicated element in HUB's centra!" << std::endl;
            continue;
        }
        centerNames.insert(centerName);
        centers->insertValue(new JValue(centerName));
    }

    return centers;
}

JObject *XMLParser::parseCenter(TiXmlElement *vaccinationCenterXML, std::ostream &errorStream, std::vector<ParseError> &errors) {
    REQUIRE(vaccinationCenterXML, "XML element corrupted");
    REQUIRE(!vaccinationCenterXML->NoChildren(), "VACCINATIECENTRUM should have children!");

    std::map<std::string, bool> parsedElements;
    parsedElements.insert(std::pair<std::string, bool>(XML_CENTER_NAME, false));
    parsedElements.insert(std::pair<std::string, bool>(XML_CENTER_ADDRESS, false));
    parsedElements.insert(std::pair<std::string, bool>(XML_CENTER_INHABITANTS, false));
    parsedElements.insert(std::pair<std::string, bool>(XML_CENTER_CAPACITY, false));

    JObject* center = new JObject();

    ITERATE_ELEMENTS(vaccinationCenterXML, element) {
        std::string elementName = element->Value();
        if(elementName == XML_CENTER_NAME || elementName == XML_CENTER_ADDRESS) {
            if(parsedElements[elementName]) {
                errors.push_back(DUPLICATED_ELEMENT);
                errorStream << "Duplicated element found in a VACCINATIECENTRUM!" << std::endl;
                continue;
            }
            parsedElements[elementName] = true;
            if(element->NoChildren()) {
                errors.push_back(EMPTY_ELEMENT);
                errorStream << "Empty element found in a VACCINATIECENTRUM!" << std::endl;
                continue;
            }
            TiXmlText* textElement = element->FirstChild()->ToText();
            if(!textElement) {
                errors.push_back(INVALID_TYPE);
                errorStream << "Element of wrong type found in a VACCINATIECENTRUM!" << std::endl;
                continue;
            }
            std::string value = textElement->Value();
            if(elementName == XML_CENTER_NAME) {
                center->insertValue(CENTER_NAME, new JValue(value));
            } else if(elementName == XML_CENTER_ADDRESS) {
                center->insertValue(CENTER_ADDRESS, new JValue(value));
            }
        } else if(elementName == XML_CENTER_INHABITANTS || elementName == XML_CENTER_CAPACITY) {
            if(parsedElements[elementName]) {
                errors.push_back(DUPLICATED_ELEMENT);
                errorStream << "Duplicated element found in a VACCINATIECENTRUM!" << std::endl;
                continue;
            }
            parsedElements[elementName] = true;
            if(element->NoChildren()) {
                errors.push_back(EMPTY_ELEMENT);
                errorStream << "Empty element found in a VACCINATIECENTRUM!" << std::endl;
                continue;
            }
            TiXmlText* textElement = element->FirstChild()->ToText();
            if(!textElement) {
                errors.push_back(INVALID_TYPE);
                errorStream << "Element of wrong type found in a VACCINATIECENTRUM!" << std::endl;
                continue;
            }
            std::string sValue = textElement->Value();
            unsigned int value;
            if(!StringUtil::stringToUnsignedInt(sValue, value)) {
                errors.push_back(INVALID_TYPE);
                errorStream << "Element of wrong type found in a VACCINATIECENTRUM!" << std::endl;
                continue;
            }
            if(elementName == XML_CENTER_INHABITANTS) {
                center->insertValue(CENTER_INHABITANTS, new JValue(value));
            } else if(elementName == XML_CENTER_CAPACITY) {
                center->insertValue(CENTER_CAPACITY, new JValue(value));
            }
        } else {
            errors.push_back(UNKNOWN_ELEMENT);
            errorStream << "Unknown element in VACCINATIECENTRUM detected!" << std::endl;
        }
    }

    if(!parsedElements[XML_CENTER_NAME]) {
        errors.push_back(MISSING_ELEMENT);
        errorStream << "Missing element: VACCINATIECENTRUM > naam" << std::endl;
    }
    if(!parsedElements[XML_CENTER_ADDRESS]) {
        errors.push_back(MISSING_ELEMENT);
        errorStream << "Missing element: VACCINATIECENTRUM > adres" << std::endl;
    }
    if(!parsedElements[XML_CENTER_CAPACITY]) {
        errors.push_back(MISSING_ELEMENT);
        errorStream << "Missing element: VACCINATIECENTRUM > capaciteit" << std::endl;
    }
    if(!parsedElements[XML_CENTER_INHABITANTS]) {
        errors.push_back(MISSING_ELEMENT);
        errorStream << "Missing element: VACCINATIECENTRUM > inwoners" << std::endl;
    }

    return center;
}
