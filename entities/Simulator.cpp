// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include <fstream>
#include "Simulator.h"
#include "../utils.h"
#include "../DesignByContract.h"
#include "../json/JObject.h"
#include "../json/JValue.h"
#include "../json/JArray.h"
#include "../json/JKeys.h"

#define C_ITERATE(type, iteratable, name) for(type::const_iterator name = iteratable.begin(); name != iteratable.end(); name++)
#define ITERATE(type, iteratable, name) for(type::iterator name = iteratable.begin(); name != iteratable.end(); name++)

// Constructors

Simulator::Simulator(): initCheck(this), initialState(NULL), daycount(0) {
    ENSURE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
}

bool Simulator::properlyInitialized() const {
    return initCheck == this;
}

// IO

void Simulator::importSimulation(const std::string& fileName) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been properly initialized!");
    REQUIRE(!fileName.empty(), "Filename cannot be empty!");
    delete initialState;
    initialState = xmlParser.parse(fileName);
    fromJSON(initialState);
    if(!isConsistent())
        throw std::runtime_error("Inconsistent simulation!");
}

void Simulator::exportSimulationSummary(const std::string& fileName) const {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(!fileName.empty(), "Filename cannot be empty!");
    std::ofstream file;
    file.open(fileName.c_str());
    assert(file.is_open());
    C_ITERATE(std::vector<Hub*>, hubs, hub) {
        (*hub)->toSummaryStream(file);
    }
    file << std::endl;
    C_ITERATE(std::vector<VaccinationCenter*>, centers, center) {
        (*center)->toSummaryStream(file);
    }
    file.close();
    ENSURE(!file.is_open(), "File wasn't closed properly!");
}

void Simulator::exportSimulationProgress(const std::string &fileName) const {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(!fileName.empty(), "Filename cannot be empty!");
    std::ofstream file;
    file.open(fileName.c_str());
    assert(file.is_open());
    C_ITERATE(std::vector<VaccinationCenter*>, centers, center) {
        (*center)->toProgressStream(file);
    }
    file.close();
    ENSURE(!file.is_open(), "File wasn't closed properly!");
}

void Simulator::fromJSON(JObject *json) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(json->contains(SIMULATION_CENTERS), StringUtil::concat("Can't load Simulator from JSON with missing field ", SIMULATION_CENTERS).c_str());
    REQUIRE(json->contains(SIMULATION_HUBS), StringUtil::concat("Can't load Simulator from JSON with missing field ", SIMULATION_HUBS).c_str());
    JValues centersJSON = json->getValue(SIMULATION_CENTERS)->asJArray()->getItems();
    ITERATE(JValues, centersJSON, center) {
        VaccinationCenter* c = new VaccinationCenter();
        c->fromJSON((*center)->asJObject());
        centers.push_back(c);
    }
    JValues hubsJSON = json->getValue(SIMULATION_HUBS)->asJArray()->getItems();
    ITERATE(JValues, hubsJSON, hub) {
        Hub* h = new Hub();
        h->fromJSON((*hub)->asJObject(), centers);
        hubs.push_back(h);
    }
    ENSURE(!hubs.empty(), "Couldn't load hubs properly!");
    ENSURE(!centers.empty(), "Couldn't load centers properly!");
}

// Simulation controls

void Simulator::run(const unsigned int cycles) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(cycles != 0, "Cycles cannot be 0!");
    REQUIRE(isConsistent(), "Simulation needs to be consistent to run!");
    unsigned int lastDay = daycount + cycles, oldDaycount = daycount;
    for(; daycount < lastDay; daycount++){
        // Deliver vaccines to the hub if expected and transport vaccines to the centers
        ITERATE(std::vector<Hub*>, hubs, hub)
            (*hub)->simulateDay(daycount);
        // Vaccinate inhabitants (should happen here to prevent double vaccinations)
        ITERATE(VaccinationCenters, centers, center)
            (*center)->vaccinateInhabitants();
    }
    ENSURE(daycount == oldDaycount + cycles, "Simulator didn't succesfully finish the right amount of cycles!");
}

bool Simulator::isConsistent() const {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    // There's at least 1 hub
    bool consistent = hubs.size() >= 1;
    // There's at least 1 center
    consistent = consistent && centers.size() >= 1;
    // All hubs are consistent
    C_ITERATE(std::vector<Hub*>, hubs, hub)
        consistent = consistent && (*hub)->isConsistent();
    // All centers are connected to at least one hub
    C_ITERATE(std::vector<VaccinationCenter*>, centers, center)
        consistent = consistent && (*center)->isConnectedToHub();
    return consistent;
}

void Simulator::reset() {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    ITERATE(std::vector<Hub*>, hubs, hub)
        delete *hub;
    ITERATE(std::vector<VaccinationCenter*>, centers, center)
        delete *center;
    fromJSON(initialState);
    daycount = 0;
}

Simulator::~Simulator() {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    delete initialState;
}
