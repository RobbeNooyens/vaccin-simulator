//
// Created by robbe on 25/02/2021.
//

#include "VaccinationCenter.h"
#include <ostream>

VaccinationCenter::VaccinationCenter(std::string name, std::string address, unsigned int inhabitants,
                                     unsigned int capacity) : name(name), address(address), inhabitants(inhabitants), capacity(capacity) {

}

std::string VaccinationCenter::getName() const {
    return name;
}

std::string VaccinationCenter::getAddress() const {
    return address;
}

unsigned int VaccinationCenter::getInhabitants() const {
    return inhabitants;
}

unsigned int VaccinationCenter::getCapacity() const {
    return capacity;
}

unsigned int VaccinationCenter::getVaccins() const {
    return vaccins;
}


void VaccinationCenter::toStream(std::ostream &outStream) const {
    outStream << getName() << ": " << getVaccinationsDone() << " gevaccineerd, nog " << getVaccinationsLeft() << " inwoners niet gevaccineerd" << std::endl;
}

unsigned int VaccinationCenter::getVaccinationsDone() const {
    return vaccinated;
}

unsigned int VaccinationCenter::getVaccinationsLeft() const {
    return inhabitants - vaccinated;
}
