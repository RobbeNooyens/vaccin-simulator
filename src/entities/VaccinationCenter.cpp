// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#include <ostream>

#include "../../DesignByContract.h"
#include "VaccinationCenter.h"
#include "../json/JObject.h"
#include "../json/JValue.h"
#include "../json/JKeys.h"
#include "../utilities/utils.h"
#include "SimulationData.h"

#define ITERATE(type, iteratable, name) for(type::iterator name = iteratable.begin(); name != iteratable.end(); name++)
#define C_ITERATE(type, iteratable, name) for(type::const_iterator name = iteratable.begin(); name != iteratable.end(); name++)
#define COMMA ,

// Constructors

VaccinationCenter::VaccinationCenter() : initCheck(this), inhabitants(0), vaccinated(0), capacity(0),
                                         connectedToHub(false) {
    ENSURE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
}

VaccinationCenter::VaccinationCenter(const std::string name, const std::string address, unsigned int inhabitants,
                                     unsigned int capacity) : initCheck(this), name(name), address(address),
                                                              inhabitants(inhabitants), vaccinated(0),
                                                              capacity(capacity), connectedToHub(false) {
    ENSURE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
}

bool VaccinationCenter::properlyInitialized() const {
    return initCheck == this;
}

// IO Streams

void VaccinationCenter::fromJSON(JObject *json) {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    REQUIRE(json != NULL, "Json can't be NULL!");
    REQUIRE(json->contains(CENTER_NAME),
            StringUtil::concat("VaccinationCenter JSON should contain field ", CENTER_NAME).c_str());
    REQUIRE(json->contains(CENTER_ADDRESS),
            StringUtil::concat("VaccinationCenter JSON should contain field ", CENTER_ADDRESS).c_str());
    REQUIRE(json->contains(CENTER_INHABITANTS),
            StringUtil::concat("VaccinationCenter JSON should contain field ", CENTER_INHABITANTS).c_str());
    REQUIRE(json->contains(CENTER_CAPACITY),
            StringUtil::concat("VaccinationCenter JSON should contain field ", CENTER_CAPACITY).c_str());
    name = json->getValue(CENTER_NAME)->asString();
    address = json->getValue(CENTER_ADDRESS)->asString();
    inhabitants = json->getValue(CENTER_INHABITANTS)->asUnsignedint();
    capacity = json->getValue(CENTER_CAPACITY)->asUnsignedint();
}

void VaccinationCenter::toSummaryStream(std::ostream &stream) const {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    REQUIRE(stream != NULL, "Output stream cannot be NULL!");
    REQUIRE(stream.good(), "Output stream contains error flags!");
    stream << getName() << ": " << getVaccinationsDone() << " gevaccineerd, nog " << getVaccinationsLeft()
           << " inwoners niet gevaccineerd" << std::endl;
    ENSURE(stream.good(), "Failed to write to parsing stream!");
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
    stream << "\t- vaccins \t\t[" << std::string(vaccinsProgressBars, '=') << std::string(20 - vaccinsProgressBars, ' ')
           << "] " << (int) (vaccinsProgress * 100) << '%' << std::endl;
    stream << "\t- vaccinated \t[" << std::string(vaccinatedProgressBars, '=')
           << std::string(20 - vaccinatedProgressBars, ' ') << "] " << (int) (vaccinatedProgress * 100) << '%'
           << std::endl;
    ENSURE(stream.good(), "Failed to write to parsing stream!");
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
    unsigned int sum = 0;
    C_ITERATE(std::map<Vaccine * COMMA unsigned int>, vaccines, vaccinePair)sum += vaccinePair->second;
    return sum;
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

void VaccinationCenter::transportationArrived(Vaccine *vaccine, unsigned int amount) {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    REQUIRE(getVaccins() + amount <= 2 * capacity, "The maximum capacity of vaccines has been exceeded!");
    unsigned int oldVaccins = getVaccins();
    vaccines[vaccine] += amount;
    ENSURE(getVaccins() == oldVaccins + amount, "Vaccines aren't added succesfully!");
}

void VaccinationCenter::vaccinateInhabitants(unsigned int day, SimulationData *statistics, std::ostream *outStream) {
    // TODO: save in SimulationData how many ottal vaccinatied
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    unsigned int totalVaccinationsDone = 0;
    // Do vaccines with renewing first
    ITERATE(std::map<Vaccine * COMMA std::map<unsigned int COMMA unsigned int> >, renewing, renew) {
        Vaccine *vaccine = renew->first;
        if (VECTOR_CONTAINS(renewing, vaccine) && VECTOR_CONTAINS(renewing[vaccine], day)) {
            unsigned int toVaccinate = renewing[vaccine][day];
            unsigned int vaccinesInStock = vaccines[vaccine];
            unsigned int vaccinsToUse = std::min(getCapacity(),
                                                 toVaccinate), oldVaccinated = getVaccinationsDone(), oldVaccins = getVaccins();
            vaccinsToUse = std::min(vaccinsToUse, vaccinesInStock);
            vaccinated += vaccinsToUse;
            vaccines[vaccine] -= vaccinsToUse;
            totalVaccinationsDone += vaccinsToUse;
            if (statistics != NULL)
                statistics->addVaccinatedInhabitants(vaccinsToUse);
            ENSURE(vaccinated == oldVaccinated + vaccinsToUse, "Vaccinated count didn't increase.");
            ENSURE(getVaccins() == oldVaccins - vaccinsToUse, "Vaccins count didn't decrease.");
        }
    }
    ITERATE(std::map<Vaccine * COMMA unsigned int>, vaccines, vaccinePair) {
        if (vaccinePair->first->getRenewing() > 0) {
            unsigned int vaccinsToUse = std::min(getVaccinationsLeft(), vaccinePair->second);
            vaccinsToUse = std::min(vaccinsToUse, getCapacity());
            renewing[vaccinePair->first].insert(
                    std::pair<unsigned int, unsigned int>(day + vaccinePair->first->getRenewing(), vaccinsToUse));
            continue;
        }
        unsigned int vaccinsToUse = std::min(getVaccinationsLeft(),
                                             vaccinePair->second), oldVaccinated = getVaccinationsDone(), oldVaccins = getVaccins();
        vaccinsToUse = std::min(vaccinsToUse, getCapacity());
        vaccinated += vaccinsToUse;
        vaccines[vaccinePair->first] -= vaccinsToUse;
        totalVaccinationsDone += vaccinsToUse;
        if (statistics != NULL)
            statistics->addVaccinatedInhabitants(vaccinsToUse);
        ENSURE(vaccinated == oldVaccinated + vaccinsToUse, "Vaccinated count didn't increase.");
        ENSURE(getVaccins() == oldVaccins - vaccinsToUse, "Vaccins count didn't decrease.");
    }
    // Output to outputstream if specified
    if (outStream != NULL)
        *outStream << "Er werden " << totalVaccinationsDone << " inwoners gevaccineerd in " << getName() << std::endl;
    removeExpiredVaccines();
}

double VaccinationCenter::getPercentageVaccines() const {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    REQUIRE(getVaccins() <= 2 * capacity, "Can't have more vaccines than twice the capacity");
    double percentage = ((double) getVaccins() / ((double) 2 * capacity));
    ENSURE(percentage >= 0 && percentage <= 1, "Percentage should be in range [0,1]");
    return percentage;
}

double VaccinationCenter::getPercentageVaccinated() const {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    REQUIRE(vaccinated <= inhabitants, "Can't have more vaccines than twice the capacity");
    double percentage = ((double) vaccinated / (double) inhabitants);
    ENSURE(percentage >= 0 && percentage <= 1, "Percentage should be in range [0,1]");
    return percentage;
}

void VaccinationCenter::setConnectedToHub(bool connected) {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    connectedToHub = connected;
}

bool VaccinationCenter::isConnectedToHub() const {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    return connectedToHub;
}

void VaccinationCenter::removeExpiredVaccines() {
    REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
    ITERATE(std::map<Vaccine * COMMA unsigned int>, vaccines, vaccinePair) {
        if (vaccinePair->first->getTemperature() < 0) {
            vaccines[vaccinePair->first] = 0;
        }
    }
}