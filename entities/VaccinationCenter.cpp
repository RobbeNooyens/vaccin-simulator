// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include <ostream>

#include "../DesignByContract.h"
#include "VaccinationCenter.h"
#include "../json/JObject.h"
#include "../json/JValue.h"

VaccinationCenter::VaccinationCenter(const std::string name, const std::string address, unsigned int inhabitants,
                                     unsigned int capacity) : initCheck(this), name(name), address(address), vaccins(0), inhabitants(inhabitants), vaccinated(0), capacity(capacity) {
    ENSURE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
}

// IO Streams

void VaccinationCenter::toStream(std::ostream &outStream) const {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
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

void VaccinationCenter::transportationArrived(unsigned int vaccinCount) {
    vaccins += vaccinCount;
}

void VaccinationCenter::vaccinateInhabitants() {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    vaccinated += std::min(capacity, vaccins);
    vaccins -= std::min(capacity, vaccins);
}

bool VaccinationCenter::properlyInitialized() const {
    return initCheck == this;
}

VaccinationCenter::VaccinationCenter(): initCheck(this), vaccins(0), inhabitants(0), vaccinated(0), capacity(0) {
    ENSURE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
}

void VaccinationCenter::fromJSON(JObject* json) {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    REQUIRE(json != NULL, "Json can't be NULL!");
    address = json->getValue("adres")->asString();
    capacity = json->getValue("capaciteit")->asUnsignedint();
    inhabitants = json->getValue("inwoners")->asUnsignedint();
    name = json->getValue("naam")->asString();
}
