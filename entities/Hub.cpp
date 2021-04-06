// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
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

#define ITERATE_OVER_CENTERS VaccinationCenters::iterator center = centers.begin(); center != centers.end(); center++
#define ITERATE_OVER_CONST_CENTERS VaccinationCenters::const_iterator center = centers.begin(); center != centers.end(); center++

Hub::Hub() : initCheck(this), delivery(0), interval(0), transport(0), vaccins(0) {
    ENSURE(properlyInitialized(), "Hub object hasn't been initialized properly!");
}

unsigned int Hub::getVaccins() const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    return vaccins;
}

void Hub::toStream(std::ostream &outStream) const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(outStream != NULL, "Output stream cannot be NULL!");
    REQUIRE(outStream.good(), "Output stream contains error flags!");
    REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
    outStream << "Hub (" << getVaccins() << "): " << std::endl;
    for(ITERATE_OVER_CONST_CENTERS)
        outStream << "  -> " << (*center)->getName() << " (" << (*center)->getVaccins() << " vaccins)" << std::endl;
    outStream << std::endl;
    for(ITERATE_OVER_CONST_CENTERS)
        (*center)->toStream(outStream);
    ENSURE(outStream.good(), "Failed to write to output stream!");
}

void Hub::simulateDay(unsigned int day) {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
    // Check if the cargo will be delivered today
    if (day % (interval+1) == 0)
        vaccins += delivery;
    // Distribute the vaccins over the centra
    distributeVaccins();
    // Vaccinate inhabitants
    for(ITERATE_OVER_CENTERS)
        (*center)->vaccinateInhabitants();
}

void Hub::transportVaccinsTo(VaccinationCenter *center, unsigned int vaccinCount) {
    REQUIRE(center != NULL, "VaccinationCenter can't be NULL!");
    center->transportationArrived(vaccinCount);
}

void Hub::distributeVaccins() {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
    std::map<VaccinationCenter*, int> vaccinsPerCenter;
    // Distribution algorithm
    // Give each center the maximum amount of vaccins it can store.
    for(ITERATE_OVER_CONST_CENTERS) {
        const unsigned int capacity = (*center)->getCapacity();
        const unsigned int vaccinsCenter = (*center)->getVaccins();
        unsigned int vaccinsTransport = 0;
        unsigned int nextVaccinsTransport = vaccinsTransport + transport;
        while(nextVaccinsTransport < vaccins && nextVaccinsTransport + vaccinsCenter <= 2*capacity) {
            vaccins -= transport;
            vaccinsTransport = nextVaccinsTransport;
            nextVaccinsTransport += transport;
        }
        if(vaccinsTransport == 0)
            vaccinsTransport = capacity;
        vaccinsPerCenter[*center] = vaccinsTransport;
    }
    // Transport vaccins
    for(ITERATE_OVER_CENTERS)
        Hub::transportVaccinsTo(*center, vaccinsPerCenter[*center]);
    ENSURE(centers.size() == vaccinsPerCenter.size(), "Map vaccinsPerCenter should contain all centers!");
}

bool Hub::properlyInitialized() const {
    return initCheck == this;
}

void Hub::fromJSON(JObject* json){
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
    REQUIRE(json != NULL, "JSON can't be NULL!");
    REQUIRE(json->contains("hub.levering"), "Hub JSON should contain field 'hub.levering'");
    REQUIRE(json->contains("hub.interval"), "Hub JSON should contain field 'hub.interval'");
    REQUIRE(json->contains("hub.transport"), "Hub JSON should contain field 'hub.transport'");
    REQUIRE(json->contains("centra"), "Hub JSON should contain field 'hub.centra'");
    delivery = json->getValue("hub.levering")->asUnsignedint();
    interval = json->getValue("hub.interval")->asUnsignedint();
    transport = json->getValue("hub.transport")->asUnsignedint();
    vaccins = delivery;
    std::vector<JValue*> centra = json->getValue("centra")->asJArray()->getItems();
    for(std::vector<JValue*>::iterator center = centra.begin(); center != centra.end(); center++) {
        VaccinationCenter* vaccinationCenter = new VaccinationCenter();
        vaccinationCenter->fromJSON((*center)->asJObject());
        centers.push_back(vaccinationCenter);
    }
    ENSURE(centra.size() == centers.size(), "Not all centers are loaded succesfully.");
}

Hub::~Hub() {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
	for (ITERATE_OVER_CENTERS)
		delete *center;
}

bool Hub::containsInvalidCenter() const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(&centers != NULL, "Centers can't be NULL!");
    for(unsigned int i = 0; i < (unsigned int) centers.size(); i++) {
        VaccinationCenter* center = centers[i];
        if(center == NULL)
            return true;
        if(!center->properlyInitialized())
            return true;
    }
    return false;
}
