// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include <ostream>

#include "../DesignByContract.h"
#include "VaccinationCenter.h"
#include "../json/JObject.h"
#include "../json/JValue.h"
#include "../json/JKeys.h"
#include "../utils.h"

// Constructors

VaccinationCenter::VaccinationCenter(): initCheck(this), vaccins(0), inhabitants(0), vaccinated(0), capacity(0), connectedToHub(false) {
    ENSURE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
}

VaccinationCenter::VaccinationCenter(const std::string name, const std::string address, unsigned int inhabitants,
                                     unsigned int capacity) : initCheck(this), name(name), address(address), vaccins(0), inhabitants(inhabitants), vaccinated(0), capacity(capacity), connectedToHub(false) {
    ENSURE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
}

bool VaccinationCenter::properlyInitialized() const {
    return initCheck == this;
}

// IO Streams

void VaccinationCenter::fromJSON(JObject* json) {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    REQUIRE(json != NULL, "Json can't be NULL!");
    REQUIRE(json->contains(CENTER_NAME), StringUtil::concat("VaccinationCenter JSON should contain field ", CENTER_NAME).c_str());
    REQUIRE(json->contains(CENTER_ADDRESS), StringUtil::concat("VaccinationCenter JSON should contain field ", CENTER_ADDRESS).c_str());
    REQUIRE(json->contains(CENTER_INHABITANTS), StringUtil::concat("VaccinationCenter JSON should contain field ", CENTER_INHABITANTS).c_str());
    REQUIRE(json->contains(CENTER_CAPACITY), StringUtil::concat("VaccinationCenter JSON should contain field ", CENTER_CAPACITY).c_str());
    name = json->getValue(CENTER_NAME)->asString();
    address = json->getValue(CENTER_ADDRESS)->asString();
    inhabitants = json->getValue(CENTER_INHABITANTS)->asUnsignedint();
    capacity = json->getValue(CENTER_CAPACITY)->asUnsignedint();
}

void VaccinationCenter::toSummaryStream(std::ostream &stream) const {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    REQUIRE(stream != NULL, "Output stream cannot be NULL!");
    REQUIRE(stream.good(), "Output stream contains error flags!");
    stream << getName() << ": " << getVaccinationsDone() << " gevaccineerd, nog " << getVaccinationsLeft() << " inwoners niet gevaccineerd" << std::endl;
    ENSURE(stream.good(), "Failed to write to output stream!");
}

void VaccinationCenter::toProgressStream(std::ostream &stream) const {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    REQUIRE(stream != NULL, "Output stream cannot be NULL!");
    REQUIRE(stream.good(), "Output stream contains error flags!");
    double vaccinsProgress = getPercentageVaccines();
    int vaccinsProgressBars = (int) (vaccinsProgress * 20);
    double vaccinatedProgress = getPercentageVaccinated();
    int vaccinatedProgressBars = (int) (vaccinatedProgress * 20);
    stream << getName() << ":" << std::endl;
    stream << "\t- vaccins \t\t[" << std::string(vaccinsProgressBars, '=') << std::string(20 - vaccinsProgressBars, ' ') << "] " << (int) (vaccinsProgress * 100) << '%' << std::endl;
    stream << "\t- vaccinated \t[" << std::string(vaccinatedProgressBars, '=') << std::string(20 - vaccinatedProgressBars, ' ') << "] " << (int) (vaccinatedProgress * 100) << '%' << std::endl;
    ENSURE(stream.good(), "Failed to write to output stream!");
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

// Simulation controls

void VaccinationCenter::transportationArrived(unsigned int vaccinCount) {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    REQUIRE(vaccins + vaccinCount <= 2*capacity, "The maximum capacity of vaccines has been exceeded!");
    unsigned int oldVaccins = vaccins;
    vaccins += vaccinCount;
    ENSURE(vaccins = oldVaccins + vaccinCount, "Vaccins aren't added succesfully!");
}

void VaccinationCenter::vaccinateInhabitants() {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    unsigned int vaccinsToUse = std::min(capacity, vaccins), oldVaccinated = vaccinated, oldVaccins = vaccins;
    vaccinated += vaccinsToUse;
    vaccins -= vaccinsToUse;
    ENSURE(vaccinated == oldVaccinated + vaccinsToUse, "Vaccinated count didn't increase.");
    ENSURE(vaccins == oldVaccins - vaccinsToUse, "Vaccins count didn't decrease.");
}

double VaccinationCenter::getPercentageVaccines() const {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    REQUIRE(vaccins <= 2*capacity, "Can't have more vaccines than twice the capacity");
    return ((double) vaccins / (double) 2*capacity);
}

double VaccinationCenter::getPercentageVaccinated() const {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    REQUIRE(vaccinated <= inhabitants, "Can't have more vaccines than twice the capacity");
    return ((double) vaccinated / (double) inhabitants);
}

void VaccinationCenter::setConnectedToHub(bool connected) {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    connectedToHub = connected;
}

bool VaccinationCenter::isConnectedToHub() const {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    return connectedToHub;
}
