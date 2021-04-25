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

#define ITERATE(type, iteratable, name) for(type::iterator name = iteratable.begin(); name != iteratable.end(); name++)
#define C_ITERATE(type, iteratable, name) for(type::const_iterator name = iteratable.begin(); name != iteratable.end(); name++)

Hub::Hub() : initCheck(this), delivery(0), interval(0), transport(0), vaccinsCount(0), connections(0) {
    ENSURE(properlyInitialized(), "Hub object hasn't been initialized properly!");
}

std::vector<Vaccine*>& Hub::getVaccins() const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    return vaccins;
}

void Hub::toSummaryStream(std::ostream & outStream) const {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
    REQUIRE(outStream != NULL, "Output stream cannot be NULL!");
    REQUIRE(outStream.good(), "Output stream contains error flags!");
    REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
    outStream << "Hub (" << getVaccins() << "): " << std::endl;
    C_ITERATE(VaccinationCenters, centers, center)
        outStream << "  -> " << (*center)->getName() << " (" << (*center)->getVaccins() << " vaccins)" << std::endl;
    outStream << std::endl;
    C_ITERATE(VaccinationCenters, centers, center)
        (*center)->toSummaryStream(outStream);
    ENSURE(outStream.good(), "Failed to write to output stream!");
}

// void Hub::simulateDay(unsigned int day) {
//     REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
//     REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
//     // Check if the cargo will be delivered today
//     if (day % (interval+1) == 0)
//         vaccinsCount += delivery;
//     // Distribute the vaccins over the centra
//     distributeVaccins();
//     // Vaccinate inhabitants
//     ITERATE(VaccinationCenters, centers, center)
//         (*center)->vaccinateInhabitants();
// }

// void Hub::transportVaccinsTo(VaccinationCenter *center, unsigned int vaccinCount) {
//     REQUIRE(center != NULL, "VaccinationCenter can't be NULL!");
//     center->transportationArrived(vaccinCount);
// }

// void Hub::distributeVaccins(std::ostream& outStream) {
//     REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
//     REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
//     std::map<VaccinationCenter*, int> vaccinsPerCenter;
//     // Distribution algorithm
//     // Give each center the maximum amount of vaccins it can store.
//     ITERATE(VaccinationCenters, centers, center) {
//         const unsigned int capacity = (*center)->getCapacity();
//         const unsigned int vaccinsCenter = (*center)->getVaccins();
//         unsigned int vaccinsTransport = 0;
//         unsigned int nextVaccinsTransport = vaccinsTransport + transport;
//         unsigned int transportationCount = 0;
//         while(nextVaccinsTransport < vaccinsCount && nextVaccinsTransport + vaccinsCenter <= 2 * capacity) {
//             transportationCount++;
//             vaccinsCount -= transport;
//             vaccinsTransport = nextVaccinsTransport;
//             nextVaccinsTransport += transport;
//         }
//         if(vaccinsTransport == 0)
//             vaccinsTransport = capacity;
//         vaccinsPerCenter[*center] = transportationCount;
//     }
//     // Transport vaccins
//     ITERATE(VaccinationCenters, centers, center) {
//         int batches = vaccinsPerCenter[*center];
//         Hub::transportVaccinsTo(*center, batches*transport);
//         outStream << "Er werden " << batches << " ladingen (" << batches*transport << " vaccins) getransporteerd naar " << (*center)->getName() << '.' << std::endl;
//     }
//     ENSURE(centers.size() == vaccinsPerCenter.size(), "Map vaccinsPerCenter should contain all centers!");
// }

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
    std::vector<JValue*> vaccs = json->getValue("vaccins")->asJArray()->getItems();
    std::vector<JValue*> centra_names = json->getValue("centra")->asJArray()->getItems();
    for (int i = 0; i < (int) vaccs.size(); i++) {
        vaccins.push_back(new Vaccine());
        vaccins[i]->fromJSON(vaccs[i]->asJObject());
        vaccins[i]->setHub(this);
    }
    for (int i = 0; i < (int) centra_names.size(); i++) {
        centers[centra_names[i]->asString()] = true;
        connections++;
    }
    ENSURE(centra.size() == centers.size(), "Not all centers are loaded succesfully.");
}

Hub::~Hub() {
    REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
}

// bool Hub::containsInvalidCenter() const {
//     REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
//     REQUIRE(&centers != NULL, "Centers can't be NULL!");
//     for(unsigned int i = 0; i < (unsigned int) centers.size(); i++) {
//         VaccinationCenter* center = centers[i];
//         if(center == NULL)
//             return true;
//         if(!center->properlyInitialized())
//             return true;
//     }
//     return false;
// }

void Hub::toProgressStream(std::ostream &outStream) const {
    C_ITERATE(VaccinationCenters, centers, center) {
        (*center)->toProgressStream(outStream);
    }
}

bool Hub::is_connected(const std::string name) const {
    return centers[name];
}

int Hub::get_connections() const {
    return connections;
}