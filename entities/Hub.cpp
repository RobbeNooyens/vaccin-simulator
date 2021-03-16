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
#include "../DesignByContract.h"
#include "../json/JArray.h"
#include "../json/JObject.h"
#include "../json/JValue.h"

#include "Hub.h"
#include "VaccinationCenter.h"

Hub::Hub() : initCheck(this), delivery(0), interval(0), transport(0), vaccins(0) {
    ENSURE(properlyInitialized(), "Hub object hasn't been initialized properly!");
}

unsigned int Hub::getVaccins() const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    return vaccins;
}

void Hub::toStream(std::ostream &outStream) const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    outStream << "Hub (" << getVaccins() << "): " << std::endl;
    VaccinationCenters::const_iterator center;
    for(center = centers.begin(); center != centers.end(); center++)
        outStream << "  -> " << (*center)->getName() << " (" << (*center)->getVaccins() << " vaccins)" << std::endl;
    outStream << std::endl;
    for(center = centers.begin(); center != centers.end(); center++)
        (*center)->toStream(outStream);
}

void Hub::simulateDay(unsigned int day) {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
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
    REQUIRE(center != NULL, "VaccinationCenter can't be NULL!");
    center->transportationArrived(vaccinCount);
}

void Hub::distributeVaccins() {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
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

void Hub::fromJSON(JObject* json){
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    delivery = json->getValue("hub")->asJObject()->getValue("levering")->asUnsignedint();
    interval = json->getValue("hub")->asJObject()->getValue("interval")->asUnsignedint();
    transport = json->getValue("hub")->asJObject()->getValue("transport")->asUnsignedint();
    std::vector<JValue*> centra = json->getValue("centra")->asJArray()->getItems();
    std::vector<JValue*>::iterator center;
    for(center = centra.begin(); center != centra.end(); center++) {
        centers.push_back(new VaccinationCenter());
        centers.back()->fromJSON((*center)->asJObject());
    }
}

Hub::~Hub() {
    VaccinationCenters::iterator center;
	for (center = centers.begin(); center != centers.end(); center++) {
		delete *center;
	}
}