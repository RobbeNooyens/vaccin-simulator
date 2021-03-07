//
// Created by robbe on 25/02/2021.
//

#include <vector>
#include <iostream>
#include <map>
#include "Hub.h"
#include "VaccinationCenter.h"

Hub::Hub() {

}

unsigned int Hub::getVaccins() const {
    return vaccins;
}

void Hub::fromStream(std::istream &inStream) {

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

}

void Hub::transportVaccinsTo(VaccinationCenter *center, unsigned int vaccinCount) {
    center->transportArrived(vaccinCount);
}

void Hub::distributeVaccins() {
    std::map<VaccinationCenter*, int> vaccinsPerCenter;
    VaccinationCenters::const_iterator center;
    for(center = centers.begin(); center != centers.end(); center++)
        vaccinsPerCenter.
    // Distribute evenly


}
