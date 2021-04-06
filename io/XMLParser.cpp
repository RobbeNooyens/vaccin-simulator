// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
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

    std::vector<std::string> elements_centra;
    elements_centra.push_back("naam");
    elements_centra.push_back("adres");
    elements_centra.push_back("inwoners");
    elements_centra.puhs_back("capaciteit");

    std::vector<std::string> elements_vaccin;
    elements_vaccin.push_back("type");
    elements_vaccin.push_back("levering");
    elements_vaccin.push_back("interval");
    elements_vaccin.push_back("transport");
    elements_vaccin.push_back("hernieuwing");
    elements_vaccin.push_back("temperatuur");

    JObject* json = new JObject;

    json->insertValue("hubs", new JValue(new JArray()));
    json->insertValue("centra", new JValue(new JArray()));

    for (TiXmlElement* elem = root->FirstChildElement("HUB"); elem != NULL; elem = elem->NextSiblingElement("HUB")) {
        JObject* _hub = new JObject;
        json->getValue("hubs")->asJArray()->insertValue(new JValue(_hub));
        _hub->insertValue("vaccins", new JValue(new JArray()));
        _hub->insertValue("centra", new JValue(new JArray()));
        TiXmlElement* nested_elem;
        TiXmlText* e_text;
        for (TiXmlElement* e = elem->FirstChildElement("VACCIN"); e != NULL; e = e->NextSiblingElement("VACCIN")) {
            JObject* _vaccin = new JObject;
            _hub->getValue("vaccins")->asJArray()->insertValue(new JArray(_vaccin));
            for (int i = 0; i < (int) elements_vaccin.size(); i++) {
                nested_elem = e->FirstChildElement(elements_vaccin[i].c_str());
                if (nested_elem == NULL) {
                    if (elements_centra[i] != "temperatuur" && elements_centra[i] != "hernieuwing") {
                        throw std::runtime_error("'" + elements_vaccin[i] + "' niet gevonden");                        
                    } else {
                        continue;
                    }
                } else {
                    e_text = nested_elem->FirstChild()->ToText();
                    if (e_text == NULL) {
                        throw std::runtime_error("waarde niet gevonden in element '" + elements_centra[i] + "'");
                    }
                    std::string value = e_text->Value();
                    if (elements_vaccin[i] == "type") {
                        _vaccin->insertValue(elements_centra[i], new JValue(value));
                    } else {
                        char *ptr;
                        std::string value = e_text->Value();
                        unsigned int k = strtoul(value.c_str(), &ptr, 10);
                        if (value.c_str() == ptr) {
                            throw std::runtime_error("waarde kon niet ingelezen worden van element '" + elements_centra[i] + "'");
                        }
                        _vaccin->insertValue(elements_centra[i], new JValue(k));
                    }
                }
            }
        }
        TiXmlElement* centras = elem->FirstChildElement("CENTRA");
        if (centras == NULL) {
            throw std::runtime_error("centra niet gevonden");
        }
        for (TiXmlElement* e = centras->FirstChildElement("centrum"); e != NULL; e = e->NextSiblingElement("centrum")) {
            e_text = e->FirstChild()->ToText();
            if (e_text == NULL) {
                throw std::runtime_error("waarde niet gevonden in element 'centrum'");
            }
            _hub->getValue("centra")->asJArray()->insertValue(new JValue(e_text->Value()));
        }
    }
    for (TiXmlElement* e = root->FirstChildElement("VACCINATIECENTRUM"); e != NULL; e = e->NextSiblingElement("VACCINATIECENTRUM")) {
        JObject* center = new JObject();
        json->getValue("centra")->asJArray()->insertValue(new JValue(center));
        for (int i = 0; i < (int) elements_centra.size(); i++) {
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
    }
    xml_document.Clear();
    return json;
}