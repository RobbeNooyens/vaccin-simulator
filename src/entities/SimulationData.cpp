//
// Created by robbe on 23/05/2021.
//

#include "SimulationData.h"
#include "../../DesignByContract.h"
#include "Vaccine.h"

#define C_ITERATE(type, iteratable, name) for(type::const_iterator name = iteratable.begin(); name != iteratable.end(); name++)
#define COMMA ,

using namespace std;

SimulationData::SimulationData(): initCheck(this), vaccinated(0) {}

void SimulationData::addDelivery(Vaccine *vaccine, unsigned int amount) {
    REQUIRE(vaccine != NULL, "Vaccine cannot be a nullpointer!");
    REQUIRE(!vaccine->getType().empty(), "Vaccine type cannot be empty!");
    if(vaccinsDelivered.find(vaccine->getType()) != vaccinsDelivered.end()) {
        vaccinsDelivered[vaccine->getType()] = amount;
    } else {
        vaccinsDelivered.insert(std::pair<std::string, unsigned int>(vaccine->getType(), amount));
    }
}

void SimulationData::addVaccinatedInhabitants(unsigned int amount) {
    vaccinated += amount;
}

void SimulationData::printStatistics(std::ostream &stream) const {
    C_ITERATE(std::map<std::string COMMA unsigned int>, vaccinsDelivered, delivered)
        stream << delivered->first << ": " << delivered->second << endl;
    stream << "Total vaccinated inhabitants: " << vaccinated << endl;
}

bool SimulationData::properlyInitialized() {
    return initCheck == this;
}

void SimulationData::reset() {
    vaccinsDelivered.clear();
    vaccinated = 0;
}
