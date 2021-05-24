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

JObject* XMLParser::parse(const std::string& fileName, std::ostream &errorStream, std::vector<ParseError> &errors) {
    REQUIRE(!fileName.empty(), "Filename can't be empty!");
    TiXmlDocument xml_document = TiXmlDocument();
    if (!xml_document.LoadFile(fileName.c_str())) {
        errorStream << xml_document.ErrorDesc() << std::endl;
        throw std::runtime_error(xml_document.ErrorDesc());
    }
    TiXmlElement* root = xml_document.FirstChildElement("ROOT");
    if (root == NULL) {
        xml_document.Clear();
        errorStream << "Failed to load file: No root element." << std::endl;
        errors.push_back(NO_ROOT);
        return NULL;
    }

    JObject* json = new JObject;
    JArray* hubs = new JArray();
    JArray* centers = new JArray();

    for (TiXmlElement* hubXml = root->FirstChildElement(XML_HUB); hubXml != NULL; hubXml = hubXml->NextSiblingElement(XML_HUB)) {
        JObject* parsedHub = parseHub(hubXml, errorStream, errors);
        hubs->insertValue(new JValue(parsedHub));
    }
    for (TiXmlElement* centerXml = root->FirstChildElement(XML_CENTER); centerXml != NULL; centerXml = centerXml->NextSiblingElement(XML_CENTER)) {
        centers->insertValue(new JValue(parseCenter(centerXml, errorStream, errors)));
    }

    json->insertValue(SIMULATION_HUBS, new JValue(hubs));
    json->insertValue(SIMULATION_CENTERS, new JValue(centers));
    xml_document.Clear();
    return json;
}

JObject *XMLParser::parseHub(TiXmlElement *hubXML, std::ostream &errorStream, std::vector<ParseError> &errors) {
    REQUIRE(hubXML != NULL, "XML element corrupted");
    JObject* hub = new JObject;

    JArray* vaccines = new JArray();
    for (TiXmlElement* element = hubXML->FirstChildElement(XML_VACCINE); element != NULL; element = element->NextSiblingElement(XML_VACCINE)) {
        vaccines->insertValue(new JValue(parseVaccin(element, errorStream, errors)));
    }

    TiXmlElement* delivery = hubXML->FirstChildElement(XML_HUB_DELIVERY);
    TiXmlElement* interval = hubXML->FirstChildElement(XML_HUB_INTERVAL);
    TiXmlElement* transportation = hubXML->FirstChildElement(XML_HUB_TRANSPORTATION);

    if (delivery || interval || transportation) {
        if(!(delivery && interval && transportation)) {
            errorStream << "Inconsistentie: Hub moet ofwel geen ofwel alledrie de velden van een standaarvaccin bevatten!" << std::endl;
            errors.push_back(INCONSISTENT_SIMULATION);
            return NULL;
        }
        JObject* vaccine = new JObject();
        vaccine->insertValue(VACCINE_DELIVERY, new JValue(XMLUtil::elementToUnsignedInt(delivery, XML_HUB_DELIVERY)));
        vaccine->insertValue(VACCINE_INTERVAL, new JValue(XMLUtil::elementToUnsignedInt(interval, XML_HUB_INTERVAL)));
        vaccine->insertValue(VACCINE_TRANSPORTATION, new JValue(XMLUtil::elementToUnsignedInt(transportation, XML_HUB_TRANSPORTATION)));
        vaccine->insertValue(VACCINE_TYPE, new JValue("Standaard"));
        vaccines->insertValue(new JValue(vaccine));
    }

    JArray* centers = parseHubCenters(hubXML->FirstChildElement(XML_HUB_CENTERS), errorStream, errors);

    hub->insertValue(HUB_VACCINES, new JValue(vaccines));
    hub->insertValue(HUB_CENTERS, new JValue(centers));
    return hub;
}

JObject *XMLParser::parseVaccin(TiXmlElement *vaccinXML, std::ostream &errorStream, std::vector<ParseError> &errors) {
    REQUIRE(vaccinXML != NULL, "XML element corrupted");

    std::vector<std::string> elements_vaccin;
    elements_vaccin.push_back(XML_VACCINE_TYPE);
    elements_vaccin.push_back(XML_VACCINE_DELIVERY);
    elements_vaccin.push_back(XML_VACCINE_INTERVAL);
    elements_vaccin.push_back(XML_VACCINE_TRANSPORTATION);
    elements_vaccin.push_back(XML_VACCINE_RENEWING);
    elements_vaccin.push_back(XML_VACCINE_TEMPERATURE);

    JObject* vaccine = new JObject;
    TiXmlElement* nested_elem;
    TiXmlText* e_text;
    for (int i = 0; i < (int) elements_vaccin.size(); i++) {
        std::string vaccineElement = elements_vaccin[i];
        nested_elem = vaccinXML->FirstChildElement(vaccineElement.c_str());
        if (nested_elem == NULL) {
            if(vaccineElement == XML_VACCINE_TEMPERATURE) {
                vaccine->insertValue(VACCINE_TEMPERATURE, new JValue((double) 0));
            } else if(vaccineElement == XML_VACCINE_RENEWING){
                vaccine->insertValue(VACCINE_RENEWING, new JValue((unsigned int) 0));
            } else {
                errors.push_back(UNKOWN_ELEMENT);
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
            errors.push_back(UNKOWN_ELEMENT);
            errorStream << "'" << centerElement << "' niet gevonden" << std::endl;
        }
    }
    return center;
}
