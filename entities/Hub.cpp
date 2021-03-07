//
// Created by robbe on 25/02/2021.
//

#include <vector>
#include <iostream>
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
