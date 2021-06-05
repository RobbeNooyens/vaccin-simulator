// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#include <vector>
#include <iostream>
#include <map>
#include <string>
#include "../../tinyxml/tinyxml.h"
#include "../../DesignByContract.h"
#include "../json/JArray.h"
#include "../json/JObject.h"
#include "../json/JValue.h"
#include "../json/JKeys.h"
#include "../utilities/utils.h"
#include "Planning.h"
#include "SimulationData.h"

#include "Hub.h"
#include "VaccinationCenter.h"

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

void Hub::toSummaryStream(std::ostream & outputStream) const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(outputStream != NULL, "Output stream cannot be NULL!");
    REQUIRE(outputStream.good(), "Output stream contains error flags!");
    REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
    outputStream << "Hub (" << getTotalVaccinesCount() << "): " << std::endl;
    C_ITERATE(VaccinationCenters, centers, center)
        outputStream << "  -> " << (*center)->getName() << " (" << (*center)->getVaccins() << " vaccins)" << std::endl;
    ENSURE(outputStream.good(), "Failed to write to parsing stream!");
}

void Hub::toProgressStream(std::ostream &outputStream) const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(outputStream != NULL, "Output stream cannot be NULL!");
    REQUIRE(outputStream.good(), "Output stream contains error flags!");
    REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
    C_ITERATE(VaccinationCenters, centers, center) {
        (*center)->toProgressStream(outputStream);
    }
    ENSURE(outputStream.good(), "Failed to write to parsing stream!");
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
    ENSURE(centerNames.size() == getVaccinationCenters().size(), "Hub's centers JSON contains an invalid name!");
    ENSURE(getVaccines().size() == vaccineCount.size(), "Vaccines vector and vaccineCount map should have the same size!");
    REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
}

// Simulation controls

void Hub::simulateDay(unsigned int day, SimulationData *statistics, std::ostream *oStream) {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(isConsistent(), "Hub needs to be consistent to run the simulation!");
    REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
    // Check if the cargo will be delivered today
    simulateDelivery(day, statistics);
    // Distribute the vaccins over the centra
    distributeVaccins();
    ENSURE(isConsistent(), "Hub needs to be consistent after running the simulation for a day!");
    ENSURE(!containsInvalidCenter(), "Hub contains an invalid center after running the simulation for a day!");
}

void Hub::simulateDelivery(unsigned int day, SimulationData *statistics) {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(isConsistent(), "Hub needs to be consistent to run the simulation");
    ITERATE(std::map<Vaccine* COMMA unsigned int>, vaccineCount, vaccinePair) {
        Vaccine* vaccine = vaccinePair->first;
        if(day % (vaccine->getInterval() + 1) == 0) {
            vaccineCount[vaccine] += vaccine->getDelivery();
            if(statistics != NULL)
                statistics->addDelivery(vaccine, vaccine->getDelivery());
        }
    }
    ENSURE(isConsistent(), "Hub needs to be consistent to run the simulation");
}

void Hub::transportVaccinsTo(VaccinationCenter *center, std::map<Vaccine *, unsigned int> loads, std::ostream *outStream = NULL) const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(center, "VaccinationCenter can't be NULL!");
    unsigned int totalLoads = 0, totalVaccines = 0;
    ITERATE(std::map<Vaccine* COMMA unsigned int>, loads, dose) {
        Vaccine* vaccine = dose->first;
        (*center).transportationArrived(vaccine, dose->second * vaccine->getTransportation());
        totalLoads += dose->second;
        totalVaccines += dose->second * vaccine->getTransportation();
    }
    if(outStream)
        *outStream << "Er werden " << totalLoads << " ladingen ( " << totalVaccines << " vaccins) getransporteerd naar " << center->getName() << "." << std::endl;
}

void Hub::distributeVaccins(std::ostream *outStream) {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(isConsistent(), "Hub needs to be consistent to run the simulation");
    REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
    ITERATE(VaccinationCenters, centers, c) {
        VaccinationCenter* center = *c;
        std::map<Vaccine*, unsigned int> vaccinePerCenter;
        unsigned int totalVaccines = 0;
        ITERATE(std::map<Vaccine* COMMA unsigned int>, vaccineCount, vaccinePair) {
            Vaccine* vaccine = vaccinePair->first;
            unsigned int nextVaccines = vaccine->getTransportation();
            unsigned int loadCount = 0;
            while(nextVaccines <= vaccinePair->second && nextVaccines + center->getVaccins() <= 2 * center->getCapacity() - totalVaccines) {
                loadCount++;
                nextVaccines += vaccine->getTransportation();
                totalVaccines += vaccine->getTransportation();
            }
            vaccinePerCenter.insert(std::pair<Vaccine*, unsigned int>(vaccine, loadCount));
        }
        Hub::transportVaccinsTo(center, vaccinePerCenter, outStream);
    }
    ENSURE(isConsistent(), "Hub needs to be consistent after running the simulation");
    ENSURE(!containsInvalidCenter(), "Hub contains an invalid center after running the simulation!");
}

void Hub::distributeEfficient(unsigned int day, Planning &planning) {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(isConsistent(), "Hub needs to be consistent to run the simulation");
    REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");

    ITERATE(VaccinationCenters, centers, c) {
        Hub::transportVaccinsTo(*c, planning.getDistribution(day, *c));
    }

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
    C_ITERATE(std::map<Vaccine* COMMA unsigned int>, vaccineCount, pair) {
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
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    bool consistent = true;
    C_ITERATE(Vaccines, vaccines, vaccin) {
        consistent = consistent && (*vaccin)->getDelivery() > 0 && (*vaccin)->getTransportation() > 0;
    }
    consistent = consistent && !centers.empty();
    C_ITERATE(VaccinationCenters, centers, center) {
        consistent = consistent && *center && (*center)->properlyInitialized();
    }
    return consistent;
    ENSURE(!containsInvalidCenter(), "Hub contains an invalid center while it's consistent!");
}

std::map<Vaccine *, unsigned int> Hub::getVaccineCount() const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    return vaccineCount;
}
