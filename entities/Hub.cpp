// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include <vector>
#include <iostream>
#include <map>
#include "../tinyxml/tinyxml.h"

#include "Hub.h"
#include "VaccinationCenter.h"

Hub::Hub() : initCheck(this), delivery(), interval(), transport(), vaccins() {}

unsigned int Hub::getVaccins() const {
    return vaccins;
}

void Hub::toStream(std::ostream &outStream) const {
    outStream << "Hub (" << getVaccins() << "): " << std::endl;
    VaccinationCenters::const_iterator center;
    for(center = centers.begin(); center != centers.end(); center++)
        outStream << "  -> " << (*center)->getName() << " (" << (*center)->getVaccins() << " vaccins)" << std::endl;
    outStream << std::endl;
    for(center = centers.begin(); center != centers.end(); center++)
        (*center)->toStream(outStream);
}

void Hub::simulateDay(unsigned int day) {
    // Check if the cargo will be delivered today
    if (day % (interval+1) == 0)
        vaccins += delivery;
    // Distribute the vaccins over the centra
    distributeVaccins();
    // Vaccinate inhabitants
    VaccinationCenters::iterator center;
    for(center = centers.begin(); center != centers.end(); center++)
        (*center)->vaccinateInhabitants();
}

void Hub::transportVaccinsTo(VaccinationCenter *center, unsigned int vaccinCount) {
    center->transportationArrived(vaccinCount);
}

void Hub::distributeVaccins() {
    std::map<VaccinationCenter*, int> vaccinsPerCenter;
    // Distribution algorithm
    // Give each center the maximum amount of vaccins it can store.
    VaccinationCenters::iterator center;
    for(center = centers.begin(); center != centers.end(); center++) {
        unsigned int amount = std::min((*center)->getCapacity(), vaccins);
        vaccins -= amount;
        vaccinsPerCenter[*center] = amount;
    }
    // Transport vaccins
    for(center = centers.begin(); center != centers.end(); center++)
        Hub::transportVaccinsTo(*center, vaccinsPerCenter[*center]);

}

bool Hub::properlyInitialized() const {
    return initCheck == this;
}

void Hub::fromTiXMLElement(TiXmlElement *element) {
	int centrum_count = 0;
	const std::string elements_hub[] = {"levering", "interval", "transport", "CENTRA"};
	const std::string elements_centra[] = {"naam", "adres", "inwoners", "capaciteit"};
	TiXmlElement* elem = element->FirstChildElement("HUB");
	if (elem == NULL) {
		throw "Hub niet gevonden.";
	} else {
		TiXmlElement* nested_elem;
		TiXmlText* e_text;
		for (int i = 0; i < elements_hub.size(); i++) {
			nested_elem = elem->FirstChildElement(elements_hub[i]);
			if (nested_elem == NULL) {
				throw elements_hub[i] + " niet gevonden.";
			} else {
				e_text = nested_elem->FirstChild()->ToText();
				if (e_text == NULL) {
					throw "waarde niet gevonden in element " + elements_hub[i];
				} else if (elements_hub[i] == "levering") {
					delivery = std::stoul(e_text->Value());
				} else if (elements_hub[i] == "interval") {
					interval = std::stoul(e_text->Value());
				} else if (elements_hub[i] == "transportation") {
					transportation = std::stoul(e_text->Value());
				} else if (elements_hub[i] == "CENTRA") {
					VaccinationCenters.push_back(new VaccinationCenter);
				}
			}
		}
	}
	for (TiXmlElement* e = element->FirstChildElement("VACCINATIECENTRUM"); e != NULL; e = e->NextSiblingElement("VACCINATIECENTRUM")) {
		TiXmlElement* nested_elem;
		TiXmlText* e_text;
		for (int i = 0; i < elements_centra.size(); i++) {
			nested_elem = e->FirstChildElement(elements_centra[i]);
			if (nested_elem == NULL) {
				throw elements_centra[i] + " niet gevonden";
			} else {
				e_text = nested_elem->FirstChild()->ToText();
				if (e_text == NULL) {
					throw "waarde niet gevonden in element " + elements_centra[i];	
				} else if (elements_centra[i] == "naam") {
					VaccinationCenters[centrum_count]->setName(e_text->Value());
				} else if (elements_centra[i] == "adres") {
					VaccinationCenters[centrum_count]->setAddress(e_text->Value());
				} else if (elements_centra[i] == "inwoners") {
					VaccinationCenters[centrum_count]->setInhabitants(std::stoul(e_text->Value()));
				} else if (elements_centra[i] == "capaciteit") {
					VaccinationCenters[centrum_count]->setCapacity(std::stoul(e_text->Value()));
				}
			}
		}
		++centrum_count;
	}
	if (centrum_count != VaccinationCenters.size()) {
		throw "niet alle vaccinatie centers zijn gelezen";
	}		
}

void Hub::fromJSON(JObject* json){

}

Hub::~Hub() {
	for (int i = 0; i < VaccinationCenters.size(); i++) {
		delete VaccinationCenters[i];
	}
}