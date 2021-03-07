//
// Created by robbe on 25/02/2021.
//

#include "Hub.h"
#include "VaccinationCenter.h"
#include <iostream>

Hub::Hub() {

}

unsigned int Hub::getVaccins() const {
    return vaccins;
}

void Hub::fromStream(std::istream &inStream) {

}

void Hub::toStream(std::ostream &outStream) const {
    outStream << "Hub (" << getVaccins() << "): " << std::endl;
    for(VaccinationCenter* center: centers)
        outStream << "  -> " << center->getName() << " (" << center->getVaccins() << " vaccins)" << std::endl;
    outStream << std::endl;
    for(VaccinationCenter* center: centers)
        center->toStream(outStream);
}
