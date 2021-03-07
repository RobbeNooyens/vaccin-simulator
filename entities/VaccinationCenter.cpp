//
// Created by robbe on 25/02/2021.
//

#include "VaccinationCenter.h"
#include <ostream>

VaccinationCenter::VaccinationCenter(const std::string name, const std::string address, unsigned int inhabitants,
                                     unsigned int capacity) : name(name), address(address), inhabitants(inhabitants), capacity(capacity) {

}

// IO Streams

void VaccinationCenter::toStream(std::ostream &outStream) const {
    outStream << getName() << ": " << getVaccinationsDone() << " gevaccineerd, nog " << getVaccinationsLeft() << " inwoners niet gevaccineerd" << std::endl;
}

// Getters

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

unsigned int VaccinationCenter::getVaccinationsDone() const {
    return vaccinated;
}

unsigned int VaccinationCenter::getVaccinationsLeft() const {
    return inhabitants - vaccinated;
}

void VaccinationCenter::transportArrived(unsigned int vaccinCount) {
    vaccins += vaccinCount;
}

void VaccinationCenter::vaccinateInhabitants() {

}
