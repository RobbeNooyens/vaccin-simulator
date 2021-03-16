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

XMLParser::XMLParser(const char* file): fileName(file){
}

void XMLParser::parseXML(Hub &hub) {
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
    hub.fromTiXMLElement(root);
    xml_document.Clear();
}

JObject* XMLParser::parse() {
    JObject* json = new JObject();
    JValue* centra = new JValue(0);
    json->put("hub", centra);
    return NULL;
}
