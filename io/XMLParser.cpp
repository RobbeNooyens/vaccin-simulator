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

XMLParser::XMLParser(const char* file): fileName(file){
}

JObject* XMLParser::parse() {
    TiXmlDocument xml_document = TiXmlDocument();
    if (!xml_document.LoadFile(fileName)) {
        std::cerr << xml_document.ErrorDesc() << std::endl;
        throw std::runtime_error(xml_document.ErrorDesc());
    }
    TiXmlElement* root = xml_document.FirstChildElement();
    if (root == NULL) {
        xml_document.Clear();
        throw std::runtime_error("Failed to load file: No root element.");
    }
    TiXmlElement* elem = xml_document.FirstChildElement("HUB");
    if (elem == NULL) {
        throw std::runtime_error("Hub niet gevonden");
    }
    JObject* json = new JObject;
    json->insertValue("hub", new JValue(new JArray));
    json->insertValue("centra", new JValue(new JArray));
    int centrum_count = 0;
    static const char* arr[] = {"levering", "interval", "transport", "CENTRA"};
    static const char* arr2[] = {"naam", "adres", "inwonders", "capaciteit"};
    std::vector<const char*> elements_hub(arr, arr+sizeof(arr)/sizeof(arr[0]));
    std::vector<const char*> elements_centra(arr2, arr2+sizeof(arr2)/sizeof(arr2[0]));
    TiXmlElement* nested_elem;
    TiXmlText* e_text;
    for (int i = 0; i < 4; i++) {
        nested_elem = elem->FirstChildElement(elements_hub[i]);
        if (nested_elem == NULL){
            throw std::runtime_error(elements_hub[i] + " niet gevonden");
        }
        e_
    }
    elem->FirstChildElement("hhhhh");
    nested_elem = elem->FirstChildElement("heell");
    std::string elements_centra[] = {"naam", "adres", "inwoners", "capaciteit"};
    //json->getValue("hub")->asJObject()->insertValue("delivery


    xml_document.Clear();

    return json;
}