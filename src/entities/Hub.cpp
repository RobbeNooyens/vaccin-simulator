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

Hub::Hub() : initCheck(this), outputStream(NULL) {
    ENSURE(properlyInitialized(), "Hub object hasn't been initialized properly!");
}

bool Hub::properlyInitialized() const {
    return initCheck == this;
}

// IO

void Hub::toSummaryStream(std::ostream & outStream) const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(outStream, "Output stream cannot be NULL!");
    REQUIRE(outStream.good(), "Output stream contains error flags!");
    REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
    outStream << "Hub (" << getTotalVaccinesCount() << "): " << std::endl;
    C_ITERATE(VaccinationCenters, centers, center)
        outStream << "  -> " << (*center)->getName() << " (" << (*center)->getVaccins() << " vaccins)" << std::endl;
    ENSURE(outStream.good(), "Failed to write to parsing stream!");
}

void Hub::toProgressStream(std::ostream &outStream) const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(outStream, "Output stream cannot be NULL!");
    REQUIRE(outStream.good(), "Output stream contains error flags!");
    REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
    C_ITERATE(VaccinationCenters, centers, center) {
        (*center)->toProgressStream(outStream);
    }
    ENSURE(outStream.good(), "Failed to write to parsing stream!");
}

void Hub::fromJSON(JObject* json, VaccinationCenters &centerList) {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(json, "JSON can't be NULL!");
    REQUIRE(json->contains(HUB_CENTERS), StringUtil::concat("Hub JSON should contain field ", HUB_CENTERS).c_str());
    REQUIRE(json->contains(HUB_VACCINES), StringUtil::concat("Hub JSON should contain field ", HUB_VACCINES).c_str());
    JValues vaccins = json->getValue(HUB_VACCINES)->asJArray()->getItems();
    ITERATE(JValues, vaccins, vaccin) {
        Vaccine* vaccine = new Vaccine();
        vaccine->fromJSON((*vaccin)->asJObject());
        vaccine->setHub(this);
        vaccines.push_back(vaccine);
        vaccineCount.insert(std::pair<Vaccine*, unsigned int>(vaccine, vaccine->getDelivery()));
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
    ENSURE(isConsistent(), "Hub isn't consistent after parsing!");
    ENSURE(json->getValue(HUB_CENTERS)->asJArray()->getItems().size() == getVaccinationCenters().size(), "Hub's centers JSON contains an invalid name!");
    ENSURE(getVaccines().size() == getVaccineCount().size(), "Vaccines vector and vaccineCount map should have the same size!");
    ENSURE(!containsInvalidCenter(), "Hub contains an invalid center!");
}

// Simulation controls

void Hub::simulateDay(unsigned int day, SimulationData &statistics) {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(isConsistent(), "Hub needs to be consistent to run the simulation!");
    REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
    // Check if the cargo will be delivered today
    simulateDelivery(day, statistics);
    // Distribute the vaccins over the centra
    distributeVaccins(statistics);
    ENSURE(isConsistent(), "Hub needs to be consistent after running the simulation for a day!");
    ENSURE(!containsInvalidCenter(), "Hub contains an invalid center after running the simulation for a day!");
}

void Hub::simulateDelivery(unsigned int day, SimulationData &statistics) {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(isConsistent(), "Hub needs to be consistent to run the simulation");
    ITERATE(std::map<Vaccine* COMMA unsigned int>, vaccineCount, vaccinePair) {
        Vaccine* vaccine = vaccinePair->first;
        if(day % (vaccine->getInterval() + 1) == 0) {
            vaccineCount[vaccine] += vaccine->getDelivery();
            statistics.addDelivery(vaccine, vaccine->getDelivery());
        }
    }
    ENSURE(isConsistent(), "Hub needs to be consistent to run the simulation");
}

void Hub::transportVaccinsTo(VaccinationCenter *center, std::map<Vaccine *, unsigned int> &loads) const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(center, "VaccinationCenter can't be NULL!");
    REQUIRE(!getOutputStream() || getOutputStream()->good(), "Output stream not available");
    unsigned int totalLoads = 0, totalVaccines = 0;
    C_ITERATE(Vaccines, vaccines, v) {
        Vaccine* vaccine = *v;
        if(vaccine->getHub() != this)
            continue;
        unsigned int dose = loads[vaccine];
        (*center).transportationArrived(vaccine, dose * vaccine->getTransportation());
        totalLoads += dose;
        totalVaccines += dose * vaccine->getTransportation();
    }
    if(outputStream)
        *outputStream << "Er werden " << totalLoads << " ladingen ( " << totalVaccines << " vaccins) getransporteerd naar " << center->getName() << "." << std::endl;
    ENSURE(center, "Center became corrupted during transport!");
}

void Hub::distributeVaccins(SimulationData &statistics) {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(isConsistent(), "Hub needs to be consistent to run the simulation");
    REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
    ITERATE(VaccinationCenters, centers, c) {
        VaccinationCenter* center = *c;
        std::map<Vaccine*, unsigned int> vaccinePerCenter;
        unsigned int totalVaccines = 0;
        ITERATE(Vaccines, vaccines, v) {
            Vaccine* vaccine = *v;
            unsigned int nextVaccines = vaccine->getTransportation();
            unsigned int loadCount = 0;
            while(nextVaccines <= vaccineCount[vaccine] && nextVaccines + center->getVaccins() <= 2 * center->getCapacity() - totalVaccines) {
                loadCount++;
                nextVaccines += vaccine->getTransportation();
                totalVaccines += vaccine->getTransportation();
            }
            vaccinePerCenter.insert(std::pair<Vaccine*, unsigned int>(vaccine, loadCount));
        }
        if(totalVaccines > 0) {
            statistics.addTransportation(this, center);
        }
        Hub::transportVaccinsTo(center, vaccinePerCenter);
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

    ENSURE(isConsistent(), "Hub needs to be consistent after running the simulation");
    ENSURE(!containsInvalidCenter(), "Hub contains an invalid center after running the simulation!");
}

bool Hub::containsInvalidCenter() const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(!getVaccinationCenters().empty(), "Centers can't be empty!");
    for(unsigned int i = 0; i < (unsigned int) centers.size(); i++) {
        if(!centers[i]->properlyInitialized())
            return true;
    }
    return false;
}

// Getters

unsigned int Hub::getTotalVaccinesCount() const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    unsigned int result = 0;
    C_ITERATE(std::map<Vaccine* COMMA unsigned int>, vaccineCount, pair) {
        result += pair->second;
    }
    return result;
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
    bool result = false;
    C_ITERATE(Vaccines, vaccines, vaccin) {
        result = result || ((*vaccin)->getInterval() > 0 && (*vaccin)->getDelivery() > 0);
    }
    result = result && !centers.empty();
    C_ITERATE(VaccinationCenters, centers, center) {
        result = result && *center && (*center)->properlyInitialized();
    }
    ENSURE(!containsInvalidCenter(), "Hub contains an invalid center while it's result!");
    return result;
}

std::map<Vaccine *, unsigned int> Hub::getVaccineCount() const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    return vaccineCount;
}

void Hub::setOutputStream(std::ostream* outStream) {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    this->outputStream = outStream;
}

// Smart simulation

bool Hub::is_connected(const std::string& name) {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    return centersMap[name];
}

int Hub::get_connections() const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    return connections;
}

unsigned int Hub::getTotalvaccins() const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    return totalvaccins;
}

std::ostream * Hub::getOutputStream() const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    return outputStream;
}
