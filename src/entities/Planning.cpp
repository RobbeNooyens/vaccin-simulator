// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#include <vector>
#include "Planning.h"
#include "../../DesignByContract.h"
#include "Hub.h"

Planning::Planning(): initCheck(this) {
    ENSURE(properlyInitialized(), "Planning object hasn't been initialized properly!");
}

Planning::~Planning() {

}

bool Planning::properlyInitialized() {
    return initCheck == this;
}

void Planning::generatePlanning(unsigned int days, std::vector<Hub*> hubs, std::vector<VaccinationCenter*> centers, unsigned int startFrom) {
    REQUIRE(properlyInitialized(), "Planning object hasn't been initialized properly!");
}

std::map<Vaccine *, unsigned int> Planning::getDistribution(unsigned int day, VaccinationCenter *center) {
    REQUIRE(properlyInitialized(), "Planning object hasn't been initialized properly!");

    return std::map<Vaccine *, unsigned int>();
}