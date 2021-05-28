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

JObject* XMLParser::parse(const std::string& fileName, std::ostream &errorStream, std::vector<ParseError> &errors) {
    REQUIRE(!fileName.empty(), "Filename can't be empty!");
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

    json->insertValue(SIMULATION_HUBS, new JValue(hubs));
    json->insertValue(SIMULATION_CENTERS, new JValue(centers));

    // TODO: Validate json

    xml_document.Clear();
    return json;
}

JObject *XMLParser::parseHub(TiXmlElement *hubXML, std::ostream &errorStream, std::vector<ParseError> &errors) {
    REQUIRE(hubXML, "XML element corrupted");
    REQUIRE(hubXML->NoChildren(), "HUB should have children!");

    JObject* hub = new JObject;
    JArray* vaccines = new JArray();
    JArray* centers = new JArray();

    std::map<std::string, unsigned int> defaultVaccine;
    std::vector<std::string> centerNames;

    ITERATE_ELEMENTS(hubXML, element) {
        std::string elementName = element->Value();

        if(element->Type() == element->TINYXML_ELEMENT) {
            if(elementName == XML_HUB_CENTERS) {
                if(element->NoChildren()) {
                    errors.push_back(EMPTY_ELEMENT);
                    errorStream << "HUB should have at least one connected center!" << std::endl;
                }
                centers = parseHubCenters(element, errorStream, errors);
            } else if(elementName == XML_VACCINE) {
                if(element->NoChildren()) {
                    errors.push_back(EMPTY_ELEMENT);
                    errorStream << "Encountered an empty element in HUB!" << std::endl;
                    continue;
                }
                vaccines->insertValue(new JValue(parseVaccin(element, errorStream, errors)));
            } else {
                errors.push_back(UNKNOWN_ELEMENT);
                errorStream << "Found an unknown element in HUB!" << std::endl;
            }

        } else if(element->FirstChild()->Type() == element->TINYXML_TEXT) {
            if(elementName == XML_HUB_DELIVERY || elementName == XML_HUB_INTERVAL || elementName == XML_HUB_TRANSPORTATION) {
                // Check for duplicated elements
                if(defaultVaccine.find(elementName) == defaultVaccine.end()) {
                    errors.push_back(DUPLICATED_ELEMENT);
                    errorStream << "Found a duplicated element in HUB!" << std::endl;
                    continue;
                }
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

    std::string type;
    unsigned int delivery = 0, interval = 0, transportation = 0, renewing = 0;
    double temperature = 0;

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
            type = textElement->Value();
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
            type = textElement->Value();
            parsedElements[XML_VACCINE_TYPE] = true;
        } else if(name == XML_VACCINE_DELIVERY || name == XML_VACCINE_INTERVAL || name == XML_VACCINE_TRANSPORTATION || name == XML_VACCINE_RENEWING) {
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
            unsigned int value;
            StringUtil::stringToUnsignedInt(sValue, value);
            if(name == XML_VACCINE_DELIVERY) {
                delivery = value;
            } else if(name == XML_VACCINE_INTERVAL) {
                interval = value;
            } else if(name == XML_VACCINE_TRANSPORTATION) {
                transportation = value;
            } else if(name == XML_VACCINE_RENEWING) {
                renewing = value;
            }
            parsedElements[name] = true;
        } else {
            errors.push_back(UNKNOWN_ELEMENT);
            errorStream << "Unknown element in VACCIN detected!" << std::endl;
        }
    }

    for (int i = 0; i < (int) elements_vaccin.size(); i++) {
        std::string vaccineElement = elements_vaccin[i];
        nested_elem = vaccinXML->FirstChildElement(vaccineElement.c_str());
        if (!nested_elem) {
            if(vaccineElement == XML_VACCINE_TEMPERATURE) {
                vaccine->insertValue(VACCINE_TEMPERATURE, new JValue((double) 0));
            } else if(vaccineElement == XML_VACCINE_RENEWING){
                vaccine->insertValue(VACCINE_RENEWING, new JValue((unsigned int) 0));
            } else {
                errors.push_back(UNKNOWN_ELEMENT);
                errorStream << "'" << vaccineElement << "' niet gevonden" << std::endl;
            }
            continue;
        }
        e_text = nested_elem->FirstChild()->ToText();
        if (e_text == NULL) {
            errors.push_back(INVALID_TYPE);
            errorStream << "waarde niet gevonden valid element '" << vaccineElement << "'" << std::endl;
            continue;
        }
        std::string value = e_text->Value();
        if (vaccineElement == XML_VACCINE_TYPE) {
            vaccine->insertValue(VACCINE_TYPE, new JValue((std::string) value));
            continue;
        }
        char *ptr;
        if (vaccineElement == XML_VACCINE_TEMPERATURE) {
            double temperature = std::strtod(value.c_str(), &ptr);
            if (value.c_str() == ptr) {
                errors.push_back(INVALID_TYPE);
                errorStream << "waarde kon niet ingelezen worden van element '" << vaccineElement << "'";
                continue;
            }
            vaccine->insertValue(VACCINE_TEMPERATURE, new JValue(temperature));
            continue;
        }
        unsigned int k = strtoul(value.c_str(), &ptr, 10);
        if (value.c_str() == ptr) {
            errors.push_back(INVALID_TYPE);
            errorStream << "waarde kon niet ingelezen worden van element '" << vaccineElement << "'";
            continue;
        }
        vaccine->insertValue(vaccineElement, new JValue(k));
    }
    return vaccine;
}

JArray *XMLParser::parseHubCenters(TiXmlElement *centraXML, std::ostream &errorStream, std::vector<ParseError> &errors) {
    REQUIRE(centraXML != NULL, "XML element corrupted");
    JArray* centers = new JArray();
    TiXmlText* centerName;
    for (TiXmlElement* element = centraXML->FirstChildElement(XML_HUB_CENTER); element != NULL; element = element->NextSiblingElement(XML_HUB_CENTER)) {
        centerName = element->FirstChild()->ToText();
        if (centerName == NULL) {
            errors.push_back(INVALID_TYPE);
            errorStream << "waarde niet gevonden valid element 'centrum'" << std::endl;
            continue;
        }
        std::string name = centerName->Value();
        centers->insertValue(new JValue(name));
    }
    return centers;
}

JObject *XMLParser::parseCenter(TiXmlElement *vaccinationCenterXML, std::ostream &errorStream, std::vector<ParseError> &errors) {
    REQUIRE(vaccinationCenterXML != NULL, "XML element corrupted");

    std::vector<std::string> elements_centra;
    elements_centra.push_back(XML_CENTER_NAME);
    elements_centra.push_back(XML_CENTER_ADDRESS);
    elements_centra.push_back(XML_CENTER_INHABITANTS);
    elements_centra.push_back(XML_CENTER_CAPACITY);

    JObject* center = new JObject();
    TiXmlElement* nested_elem;
    TiXmlText* e_text;
    for (int i = 0; i < (int) elements_centra.size(); i++) {
        std::string centerElement = elements_centra[i];
        nested_elem = vaccinationCenterXML->FirstChildElement(centerElement.c_str());
        if (nested_elem == NULL) {
            errors.push_back(INVALID_TYPE);
            errorStream << "'" << centerElement << "' niet gevonden";
            continue;
        }
        e_text = nested_elem->FirstChild()->ToText();
        if (e_text == NULL) {
            errors.push_back(INVALID_TYPE);
            errorStream << "waarde niet gevonden valid element '" << centerElement << "'";
            continue;
        }
        std::string value = e_text->Value();
        if (centerElement == XML_CENTER_INHABITANTS || centerElement == XML_CENTER_CAPACITY) {
            char *ptr;
            unsigned int k = strtoul(value.c_str(), &ptr, 10);
            if (value.c_str() == ptr) {
                errors.push_back(INVALID_TYPE);
                errorStream << "waarde kon niet ingelezen worden van element '" << centerElement << "'" << std::endl;
            }
            center->insertValue(centerElement, new JValue(k));
        } else if(centerElement == XML_CENTER_NAME || centerElement == XML_CENTER_ADDRESS) {
            center->insertValue(centerElement, new JValue(value));
        } else {
            errors.push_back(UNKNOWN_ELEMENT);
            errorStream << "'" << centerElement << "' niet gevonden" << std::endl;
        }
    }
    return center;
}
