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

VaccinationCenter::VaccinationCenter() : initCheck(this) {
    ENSURE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
}

// IO Streams

void VaccinationCenter::fromJSON(JObject* json) {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    REQUIRE(json != NULL, "Json can't be NULL!");
    REQUIRE(json->contains("naam"), "VaccinationCenter JSON should contain field 'naam'");
    REQUIRE(json->contains("adres"), "VaccinationCenter JSON should contain field 'adres'");
    REQUIRE(json->contains("inwoners"), "VaccinationCenter JSON should contain field 'inwoners'");
    REQUIRE(json->contains("capaciteit"), "VaccinationCenter JSON should contain field 'capaciteit'");
    address = json->getValue("adres")->asString();
    capacity = json->getValue("capaciteit")->asUnsignedint();
    inhabitants = json->getValue("inwoners")->asUnsignedint();
    name = json->getValue("naam")->asString();
    totalInhabitants = inhabitants;
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
    double vaccinsProgress = std::min((double) 100, ((double) vaccins / capacity));
    int vaccinsProgressBars = (int) (vaccinsProgress * 20);
    double vaccinatedProgress = std::min((double) 100, ((double) vaccinated / inhabitants));
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

void VaccinationCenter::transportationArrived(unsigned int vaccinCount) {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
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

bool VaccinationCenter::properlyInitialized() const {
    return initCheck == this;
}

VaccinationCenter::VaccinationCenter(): initCheck(this), vaccins(0), inhabitants(0), vaccinated(0), capacity(0) {
    ENSURE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
}

bool VaccinationCenter::operator<(const VaccinationCenter& s) const {
    return capacity < s.getCapacity();
}

std::vector<std::pair<unsigned int, unordered_map<Vaccine*, unsigned int>>>& VaccinationCenter::getCalender() const {
    return vaccine_calender;
}
