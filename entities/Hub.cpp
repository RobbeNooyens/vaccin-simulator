// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include <vector>
#include <iostream>
#include <map>

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
    center->transportArrived(vaccinCount);
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

}
