// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include <iostream>
#include "XMLParser.h"
#include "../entities/Hub.h"
#include "../tinyxml/tinyxml.h"
#include "../json/JObject.h"
#include "../json/JValue.h"
#include "../json/JArray.h"
#include "../DesignByContract.h"

XMLParser::XMLParser() : initCheck(this) {
    ENSURE(properlyInitialized(), "XMLParser object hasn't been initialized properly!");
}

bool XMLParser::properlyInitialized() const {
    return initCheck == this;
}

JObject* XMLParser::parse(const std::string& fileName) {
    REQUIRE(properlyInitialized(), "XMLParser object hasn't been initialized properly!");
    TiXmlDocument xml_document = TiXmlDocument();
    if (!xml_document.LoadFile(fileName.c_str())) {
        std::cerr << xml_document.ErrorDesc() << std::endl;
        throw std::runtime_error(xml_document.ErrorDesc());
    }
    TiXmlElement* root = xml_document.FirstChildElement();
    if (root == NULL) {
        xml_document.Clear();
        throw std::runtime_error("Failed to load file: No root element.");
    }
    TiXmlElement* elem = root->FirstChildElement("HUB");
    if (elem == NULL) {
        throw std::runtime_error("Hub not found");
    }
    JObject* json = new JObject;
    unsigned int count = 0;
    json->insertValue("hub", new JValue(new JObject()));
    json->insertValue("centra", new JValue(new JArray()));
    std::vector<std::string> elements_hub;
    std::vector<std::string> elements_centra;
    elements_hub.push_back("levering");
    elements_hub.push_back("interval");
    elements_hub.push_back("transport");
    elements_centra.push_back("naam");
    elements_centra.push_back("adres");
    elements_centra.push_back("inwoners");
    elements_centra.push_back("capaciteit");
    TiXmlElement* nested_elem;
    TiXmlText* e_text;
    for (int i = 0; i < (int) elements_hub.size(); i++) {
        nested_elem = elem->FirstChildElement(elements_hub[i].c_str());
        if (nested_elem == NULL) {
            throw std::runtime_error("'" + elements_hub[i] + "' niet gevonden");
        } else {
            e_text = nested_elem->FirstChild()->ToText();
            if (e_text == NULL) {
                throw std::runtime_error("waarde niet gevonden in element '" + elements_hub[i] + "'");
            }
            char *ptr;
            std::string value = e_text->Value();
            unsigned int k = strtoul(value.c_str(), &ptr, 10);
            if (value.c_str() == ptr) {
                throw std::runtime_error("waarde kon niet ingelezen worden van element '" + elements_hub[i] + "'");
            }
            json->getValue("hub")->asJObject()->insertValue(elements_hub[i], new JValue(k));
        }
    }
    elem = xml_document.FirstChildElement("VACCINATIECENTRUM");
    for (TiXmlElement* e = root->FirstChildElement("VACCINATIECENTRUM"); e != NULL; e = e->NextSiblingElement("VACCINATIECENTRUM")) {
        JObject* center = new JObject();
        json->getValue("centra")->asJArray()->insertValue(new JValue(center));
        for (int i = 0; i < 4; i++) {
            nested_elem = e->FirstChildElement(elements_centra[i].c_str());
            if (nested_elem == NULL) {
                throw std::runtime_error("'" + elements_centra[i] + "' niet gevonden");
            } else {
                e_text = nested_elem->FirstChild()->ToText();
                if (e_text == NULL) {
                    throw std::runtime_error("waarde niet gevonden in element '" + elements_centra[i] + "'");
                }
                std::string value = e_text->Value();
                if (elements_centra[i] == "inwoners" || elements_centra[i] == "capaciteit") {
                    char *ptr;
                    unsigned int k = strtoul(value.c_str(), &ptr, 10);
                    if (value.c_str() == ptr) {
                        throw std::runtime_error("waarde kon niet ingelezen worden van element '" + elements_centra[i] + "'");
                    }
                    center->insertValue(elements_centra[i], new JValue(k));
                } else {
                    center->insertValue(elements_centra[i], new JValue(value));
                }
            }
        }
        ++count;
    }
    xml_document.Clear();
    return json;
}