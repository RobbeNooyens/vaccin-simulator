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
    REQUIRE(outStream != NULL, "Output stream cannot be NULL!");
    REQUIRE(outStream.good(), "Output stream contains error flags!");
    outStream << getName() << ": " << getVaccinationsDone() << " gevaccineerd, nog " << getVaccinationsLeft() << " inwoners niet gevaccineerd" << std::endl;
    ENSURE(outStream.good(), "Failed to write to output stream!");
}

// Getters

std::string VaccinationCenter::getName() const {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    return name;
}

std::string VaccinationCenter::getAddress() const {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    return address;
}

unsigned int VaccinationCenter::getInhabitants() const {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    return inhabitants;
}

unsigned int VaccinationCenter::getCapacity() const {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    return capacity;
}

unsigned int VaccinationCenter::getVaccins() const {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    return vaccins;
}

unsigned int VaccinationCenter::getVaccinationsDone() const {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    return vaccinated;
}

unsigned int VaccinationCenter::getVaccinationsLeft() const {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    return inhabitants - vaccinated;
}

void VaccinationCenter::transportationArrived(unsigned int vaccinCount) {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    vaccins += vaccinCount;
}

void VaccinationCenter::vaccinateInhabitants() {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    unsigned int vaccinsToUse = std::min(capacity, vaccins), oldVaccinated = vaccinated, oldVaccins = vaccins;
    vaccinated += vaccinsToUse;
    vaccins -= vaccinsToUse;
    ENSURE(vaccinated = oldVaccinated + vaccinsToUse, "Vaccinated count didn't increase.");
    ENSURE(vaccins = oldVaccins - vaccinsToUse, "Vaccins count didn't decrease.");
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
    REQUIRE(json->contains("adres"), "VaccinationCenter JSON should contain field 'adres'");
    REQUIRE(json->contains("capaciteit"), "VaccinationCenter JSON should contain field 'capaciteit'");
    REQUIRE(json->contains("inwoners"), "VaccinationCenter JSON should contain field 'inwoners'");
    REQUIRE(json->contains("naam"), "VaccinationCenter JSON should contain field 'naam'");
    address = json->getValue("adres")->asString();
    capacity = json->getValue("capaciteit")->asUnsignedint();
    inhabitants = json->getValue("inwoners")->asUnsignedint();
    name = json->getValue("naam")->asString();
}
