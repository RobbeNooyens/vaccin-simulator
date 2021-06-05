// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#include <fstream>
#include "Simulator.h"
#include "../utilities/utils.h"
#include "../../DesignByContract.h"
#include "../json/JObject.h"
#include "../json/JValue.h"
#include "../json/JArray.h"
#include "../json/JKeys.h"
#include "Planning.h"

#define C_ITERATE(type, iteratable, name) for(type::const_iterator name = iteratable.begin(); name != iteratable.end(); name++)
#define ITERATE(type, iteratable, name) for(type::iterator name = iteratable.begin(); name != iteratable.end(); name++)

// Constructors

Simulator::Simulator(): initCheck(this), initialState(NULL), daycount(0), statisticsOutputStream(NULL), transportationOutputStream(NULL), vaccinationOutputStream(NULL) {
    ENSURE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
}

Simulator::~Simulator() {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    delete initialState;
}

bool Simulator::properlyInitialized() const {
    return initCheck == this;
}

// IO

void Simulator::importSimulation(const std::string &fileName, std::ostream &errorStream) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been properly initialized!");
    REQUIRE(!fileName.empty(), "Filename cannot be empty!");
    delete initialState;
    ParseErrors errors;
    initialState = XMLParser::parse(fileName, errorStream, errors);
    if(!errors.empty())
        throw std::runtime_error("An error occured during parsing!");
    fromJSON(initialState);
}

void Simulator::exportSimulationSummary(std::ostream &out) const {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(out.good(), "Output stream should be writeable!");
    C_ITERATE(std::vector<Hub*>, hubs, hub) {
        (*hub)->toSummaryStream(out);
    }
    out << std::endl;
    C_ITERATE(std::vector<VaccinationCenter*>, centers, center) {
        (*center)->toSummaryStream(out);
    }
    ENSURE(out.good(), "Stream encountered an error during writing!");
}

void Simulator::exportSimulationProgress(std::ostream &out) const {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(out.good(), "Output stream should be writeable!");
    C_ITERATE(std::vector<VaccinationCenter*>, centers, center) {
        (*center)->toProgressStream(out);
    }
    ENSURE(out.good(), "Stream encountered an error during writing!");
}

void Simulator::exportSimulationIniFile(std::ostream &out) const {
    REQUIRE(properlyInitialized(), "Object hasn't been initialized properly!");
    REQUIRE(out.good(), "Output stream should be writeable!");

    // TODO: ini file generation

    ENSURE(out.good(), "Stream encountered an error during writing!");
}

void Simulator::fromJSON(JObject *json) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(json->contains(SIMULATION_CENTERS), StringUtil::concat("Can't load Simulator from JSON with missing field ", SIMULATION_CENTERS).c_str());
    REQUIRE(json->contains(SIMULATION_HUBS), StringUtil::concat("Can't load Simulator from JSON with missing field ", SIMULATION_HUBS).c_str());
    REQUIRE(ParseUtil::isConsistent(json), "Simulation JSON should be consistent!");
    JValues centersJSON = json->getValue(SIMULATION_CENTERS)->asJArray()->getItems();
    ITERATE(JValues, centersJSON, center) {
        VaccinationCenter* c = new VaccinationCenter();
        c->fromJSON((*center)->asJObject());
        c->setOutputStream(vaccinationOutputStream);
        centers.push_back(c);
    }
    JValues hubsJSON = json->getValue(SIMULATION_HUBS)->asJArray()->getItems();
    ITERATE(JValues, hubsJSON, hub) {
        Hub* h = new Hub();
        h->fromJSON((*hub)->asJObject(), centers);
        h->setOutputStream(transportationOutputStream);
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
    unsigned int inhabitants = 0;
    ITERATE(VaccinationCenters, centers, center) {
        inhabitants += (*center)->getInhabitants();
    }
    while(daycount < lastDay){
        // Deliver vaccines to the hub if expected and transport vaccines to the centers
        ITERATE(std::vector<Hub*>, hubs, hub)(*hub)->simulateDay(daycount, statistics);
        // Vaccinate inhabitants (should happen here to prevent double vaccinations)
        ITERATE(VaccinationCenters, centers, center)(*center)->vaccinateInhabitants(daycount, statistics);
        if(statisticsOutputStream)
            statistics.printStatistics(*statisticsOutputStream);
        daycount++;
    }
    ENSURE(daycount == oldDaycount + cycles, "Simulator didn't succesfully finish the right amount of cycles!");
}

void Simulator::runEfficient(unsigned int cycles) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(cycles != 0, "Cycles cannot be 0!");
    REQUIRE(isConsistent(), "Simulation needs to be consistent to run!");
    Planning planning = Planning();
    planning.generatePlanning(cycles, hubs, centers, daycount);

    unsigned int lastDay = daycount + cycles;
    while(daycount < lastDay) {
        // Deliver vaccines to the hub if expected and transport vaccines to the centers
        ITERATE(std::vector<Hub*>, hubs, hub) {
            (*hub)->simulateDelivery(daycount, statistics);
            (*hub)->distributeEfficient(daycount, planning);
        }
        // Vaccinate inhabitants (should happen here to prevent double vaccinations)
        ITERATE(VaccinationCenters, centers, center)(*center)->vaccinateInhabitants(daycount, statistics);
        daycount++;
    }
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
    daycount = 0;
    fromJSON(initialState);
}

void Simulator::setStatisticsStream(std::ostream *stats) {
    this->statisticsOutputStream = stats;
}

void Simulator::setTransportationStream(std::ostream* transport) {
    this->transportationOutputStream = transport;
    ITERATE(std::vector<Hub*>, hubs, hub) {
        (*hub)->setOutputStream(transport);
    }
}

void Simulator::setVaccinationsStream(std::ostream *vaccinations) {
    this->vaccinationOutputStream = vaccinations;
    ITERATE(VaccinationCenters, centers, center) {
        (*center)->setOutputStream(vaccinations);
    }
}
