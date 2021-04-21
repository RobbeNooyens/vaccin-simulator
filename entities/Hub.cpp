// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include <vector>
#include <iostream>
#include <map>
#include <string>
#include "../tinyxml/tinyxml.h"
#include "../DesignByContract.h"
#include "../json/JArray.h"
#include "../json/JObject.h"
#include "../json/JValue.h"
#include "../json/JKeys.h"
#include "../utils.h"

#include "Hub.h"
#include "VaccinationCenter.h"
#include "Vaccine.h"

#define ITERATE(type, iteratable, name) for(type::iterator name = iteratable.begin(); name != iteratable.end(); name++)
#define C_ITERATE(type, iteratable, name) for(type::const_iterator name = iteratable.begin(); name != iteratable.end(); name++)
#define COMMA ,

// Constructors, destructors and initialization

Hub::Hub() : initCheck(this) {
    ENSURE(properlyInitialized(), "Hub object hasn't been initialized properly!");
}

bool Hub::properlyInitialized() const {
    return initCheck == this;
}

// IO

void Hub::toSummaryStream(std::ostream & outStream) const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(outStream != NULL, "Output stream cannot be NULL!");
    REQUIRE(outStream.good(), "Output stream contains error flags!");
    REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
    outStream << "Hub (" << getTotalVaccinesCount() << "): " << std::endl;
    C_ITERATE(VaccinationCenters, centers, center)
        outStream << "  -> " << (*center)->getName() << " (" << (*center)->getVaccins() << " vaccins)" << std::endl;
//    outStream << std::endl;
//    C_ITERATE(VaccinationCenters, centers, center)
//        (*center)->toSummaryStream(outStream);
    ENSURE(outStream.good(), "Failed to write to output stream!");
}

void Hub::toProgressStream(std::ostream &outStream) const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    C_ITERATE(VaccinationCenters, centers, center) {
        (*center)->toProgressStream(outStream);
    }
}

void Hub::fromJSON(JObject* json, VaccinationCenters &centerList){
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
    REQUIRE(json != NULL, "JSON can't be NULL!");
    REQUIRE(json->contains(HUB_CENTERS), StringUtil::concat("Hub JSON should contain field ", HUB_CENTERS).c_str());
    REQUIRE(json->contains(HUB_VACCINES), StringUtil::concat("Hub JSON should contain field ", HUB_VACCINES).c_str());
    JValues vaccins = json->getValue(HUB_VACCINES)->asJArray()->getItems();
    ITERATE(JValues, vaccins, vaccin) {
        // TODO: replace string fields with macros or static variables
        Vaccine* vaccine = new Vaccine();
        vaccine->fromJSON((*vaccin)->asJObject());
        vaccines.push_back(vaccine);
        vaccineCount.insert(std::pair<Vaccine*, int>(vaccine, vaccine->getDelivery()));
    }
    std::vector<JValue*> centerNames = json->getValue(HUB_CENTERS)->asJArray()->getItems();
    ITERATE(std::vector<JValue*>, centerNames, name) {
        std::string centerName = (*name)->asString();
        ITERATE(VaccinationCenters, centerList, center) {
            if((*center)->getName() == centerName) {
                centers.push_back(*center);
                (*center)->setConnectedToHub(true);
                continue;
            }
        }
    }
    ENSURE(centerNames.size() == centers.size(), "Hub's centers JSON contains an invalid name!");
    ENSURE(vaccines.size() == vaccineCount.size(), "Vaccines vector and vaccineCount map should have the same size!");
}

// Simulation controls

void Hub::simulateDay(unsigned int day) {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
    // Check if the cargo will be delivered today
    // TODO: replace with vaccines fields
//    if (day % (interval+1) == 0)
//        vaccinsCount += delivery;
    // Distribute the vaccins over the centra
    distributeVaccins();
    // Vaccinate inhabitants
    ITERATE(VaccinationCenters, centers, center)
        (*center)->vaccinateInhabitants();
}

void Hub::transportVaccinsTo(VaccinationCenter *center, unsigned int vaccinCount) {
    REQUIRE(center != NULL, "VaccinationCenter can't be NULL!");
    center->transportationArrived(vaccinCount);
}

void Hub::distributeVaccins(std::ostream& outStream) {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
    std::map<VaccinationCenter*, int> vaccinsPerCenter;
    ITERATE(VaccinationCenters, centers, center) {
        vaccinsPerCenter.insert(std::pair<VaccinationCenter*, int>(*center, 1000));
        Hub::transportVaccinsTo(*center, 1000);
    }
    // Distribution algorithm
    // Give each center the maximum amount of vaccins it can store.
    // TODO: replace with vaccines fields
//    ITERATE(VaccinationCenters, centers, center) {
//        const unsigned int capacity = (*center)->getCapacity();
//        const unsigned int vaccinsCenter = (*center)->getTotalVaccinesCount();
//        unsigned int vaccinsTransport = 0;
//        unsigned int nextVaccinsTransport = vaccinsTransport + transport;
//        unsigned int transportationCount = 0;
//        while(nextVaccinsTransport < vaccinsCount && nextVaccinsTransport + vaccinsCenter <= 2 * capacity) {
//            transportationCount++;
//            vaccinsCount -= transport;
//            vaccinsTransport = nextVaccinsTransport;
//            nextVaccinsTransport += transport;
//        }
//        if(vaccinsTransport == 0)
//            vaccinsTransport = capacity;
//        vaccinsPerCenter[*center] = transportationCount;
//    }
//    // Transport vaccins
//    ITERATE(VaccinationCenters, centers, center) {
//        int batches = vaccinsPerCenter[*center];
//        Hub::transportVaccinsTo(*center, batches*transport);
//        outStream << "Er werden " << batches << " ladingen (" << batches*transport << " vaccins) getransporteerd naar " << (*center)->getName() << '.' << std::endl;
//    }
    ENSURE(centers.size() == vaccinsPerCenter.size(), "Map vaccinsPerCenter should contain all centers!");
}

bool Hub::containsInvalidCenter() const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(&centers != NULL, "Centers can't be NULL!");
    for(unsigned int i = 0; i < (unsigned int) centers.size(); i++) {
        if(!centers[i]->properlyInitialized())
            return true;
    }
    return false;
}

// Getters

unsigned int Hub::getTotalVaccinesCount() const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    unsigned int count = 0;
    C_ITERATE(std::map<Vaccine* COMMA int>, vaccineCount, pair) {
        count += pair->second;
    }
    return count;
}

VaccinationCenters Hub::getVaccinationCenters() const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    return centers;
}

Vaccines Hub::getVaccines() const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    return vaccines;
}

bool Hub::isConsistent() const {
    bool consistent = true;
    C_ITERATE(Vaccines, vaccines, vaccin) {
        consistent = consistent && (*vaccin)->getDelivery() > 0 && (*vaccin)->getTransportation() > 0;
    }
    consistent = consistent && centers.size() > 0;
    C_ITERATE(VaccinationCenters, centers, center) {
        consistent = consistent && *center && (*center)->properlyInitialized();
    }
    return consistent;
}
