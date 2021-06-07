// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#include "SimulationData.h"
#include "../../DesignByContract.h"
#include "Vaccine.h"

#define C_ITERATE(type, iteratable, name) for(type::const_iterator name = iteratable.begin(); name != iteratable.end(); name++)
#define COMMA ,

using namespace std;

SimulationData::SimulationData(): initCheck(this), vaccinated(0) {
    ENSURE(properlyInitialized(), "SimulationData wasn't initialized properly!");
}

void SimulationData::addDelivery(Vaccine *vaccine, unsigned int amount) {
    REQUIRE(properlyInitialized(), "SimulationData wasn't initialized properly!");
    REQUIRE(vaccine, "Vaccine cannot be a nullpointer!");
    REQUIRE(!vaccine->getType().empty(), "Vaccine type cannot be empty!");
    if(vaccinsDelivered.find(vaccine->getType()) != vaccinsDelivered.end()) {
        vaccinsDelivered[vaccine->getType()] += amount;
    } else {
        vaccinsDelivered.insert(std::pair<std::string, unsigned int>(vaccine->getType(), amount));
    }
    ENSURE(getVaccinsDelivered().at(vaccine->getType()) >= amount, "Didn't succesfully add the delivery to the simulationdata!");
}

void SimulationData::addVaccinatedInhabitants(unsigned int amount) {
    REQUIRE(properlyInitialized(), "SimulationData wasn't initialized properly!");
    vaccinated += amount;
    ENSURE(getAmountVaccinated() >= amount, "Didn't succesfully add the new amount of vaccinated inhabitatns!");
}

void SimulationData::printStatistics(std::ostream &stream) const {
    REQUIRE(properlyInitialized(), "SimulationData wasn't initialized properly!");
    REQUIRE(stream.good(), "Stream to print statistics to is in an error state!");
    C_ITERATE(std::map<std::string COMMA unsigned int>, vaccinsDelivered, delivered) {
        stream << delivered->first << ": " << delivered->second << endl;
    }
    stream << "Total vaccinated inhabitants: " << vaccinated << endl;
    REQUIRE(stream.good(), "Printing the statistics ended up in an error state!");
}

bool SimulationData::properlyInitialized() const {
    return initCheck == this;
}

void SimulationData::reset() {
    REQUIRE(properlyInitialized(), "SimulationData wasn't initialized properly!");
    vaccinsDelivered.clear();
    vaccinated = 0;
    ENSURE(getVaccinsDelivered().empty(), "SimulationData didn't reset well!");
    ENSURE(getAmountVaccinated() == 0, "SimulationData didn't reset well!");
}

unsigned int SimulationData::getAmountVaccinated() const {
    REQUIRE(properlyInitialized(), "SimulationData wasn't initialized properly!");
    return vaccinated;
}

std::map<std::string, unsigned int> &SimulationData::getVaccinsDelivered() {
    REQUIRE(properlyInitialized(), "SimulationData wasn't initialized properly!");
    return vaccinsDelivered;
}

void SimulationData::addTransportation(Hub *hub, VaccinationCenter *center) {
    REQUIRE(properlyInitialized(), "SimulationData wasn't initialized properly!");
    REQUIRE(hub, "Hub cannot be NULL!");
    REQUIRE(center, "Center cannot be NULL!");
    if(transportations.find(hub) == transportations.end()) {
        std::vector<VaccinationCenter*> centers;
        centers.push_back(center);
        transportations.insert(std::pair<Hub*, std::vector<VaccinationCenter*> >(hub, centers));
    } else {
        transportations[hub].push_back(center);
    }
    ENSURE(getTransportations().find(hub) == getTransportations().end(), "Hub should be added to the map!");
}

std::map<Hub *, std::vector<VaccinationCenter *> > &SimulationData::getTransportations() {
    REQUIRE(properlyInitialized(), "SimulationData wasn't initialized properly!");
    return transportations;
}

