// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#ifndef VACCIN_SIMULATOR_SIMULATOR_H
#define VACCIN_SIMULATOR_SIMULATOR_H

#include "../parsing/XMLParser.h"
#include "VaccinationCenter.h"
#include "Hub.h"
#include "SimulationData.h"
#include <string>
#include <vector>

class JObject;

class Simulator {
public:
    // Constructor
    /**
     * Simulator default constructor
     * * ENSURE(properlyInitialized(), "Object hasn't been initialized properly!");
     */
    Simulator();
    /**
     * Simulator destructor
     * * REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
     */
    ~Simulator();

    // Initialization
    /**
     * Checks if the current object was initialized properly
     * @return bool; true if the initCheck pointer points to the current instance
     */
    bool properlyInitialized() const;

    // Set output streams

    void setStatisticsStream(std::ostream* stats);

    void setTransportationStream(std::ostream* transport);

    void setVaccinationsStream(std::ostream* vaccinations);

    // IO
    /**
     * Imports the simulation from an XML file
     * @param fileName: name of the XML file with extension
     * * REQUIRE(properlyInitialized(), "Object hasn't been properly initialized!");
     * * REQUIRE(!fileName.empty(), "Filename cannot be empty!");
     */
    void importSimulation(const std::string &fileName, std::ostream &errorStream);
    /**
     * Exports the simulation summary to a text file
     * @param out: name of the text file to write to
     * * REQUIRE(properlyInitialized(), "Object hasn't been initialized properly!");
     * * REQUIRE(!fileName.empty(), "Filename cannot be empty!");
     * * ENSURE(!file.is_open(), "File wasn't closed properly!");
     */
    void exportSimulationSummary(std::ostream &out) const;
    /**
     * Exports the simulation progress to a text file
     * @param out: name of the text file to write to
     * * REQUIRE(properlyInitialized(), "Object hasn't been initialized properly!");
     * * REQUIRE(!fileName.empty(), "Filename cannot be empty!");
     * * ENSURE(!file.is_open(), "File wasn't closed properly!");
     */
    void exportSimulationProgress(std::ostream &out) const;
    /**
     * Writes contents of the objects valid the simulation to an ini file
     * @param out: string; name of the file to write to
     * * REQUIRE(properlyInitialized(), "Object hasn't been initialized properly!");
     * * REQUIRE(StringUtil::contains(fileName, ".ini"), "File should be an ini file!");
     * * REQUIRE(!fileName.empty(), "Filename cannot be empty!");
     * * ENSURE(!file.is_open(), "File wasn't closed properly!");
     */
    void exportSimulationIniFile(std::ostream &out) const;
    /**
     *
     * @param json
     * * REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
     * * REQUIRE(json->contains("centra"), "Can't load Simulator from JSON with missing field 'centra'");
     * * REQUIRE(json->contains("hubs"), "Can't load Simulator from JSON with missing field 'hubs'");
     * * REQUIRE(ParseUtil::isConsistent(json), "Simulation JSON should be consistent!");
     * * ENSURE(!hubs.empty(), "Couldn't load hubs properly!");
     * * ENSURE(!centers.empty(), "Couldn't load centers properly!");
     */
    void fromJSON(JObject* json);

    // Controls
    /**
     * Runs the simulation for a specific amount of times
     * @param cycles: the amount of times the simulation should run
     * * REQUIRE(properlyInitialized(), "Object hasn't been initialized properly!");
     * * REQUIRE(cycles != 0, "Cycles cannot be 0!");
     * * REQUIRE(isConsistent(), "Simulation needs to be consistent to run!");
     * * ENSURE(daycount == oldDaycount + cycles, "Simulator didn't succesfully finish the right amount of cycles!");
     */
    void run(unsigned int cycles);
    /**
     * Runs the simulation for a specific amount of times with smart distribution
     * @param startFromDay: int; the day to start from
     * * REQUIRE(properlyInitialized(), "Object hasn't been initialized properly!");
     * * REQUIRE(cycles != 0, "Cycles cannot be 0!");
     * * REQUIRE(isConsistent(), "Simulation needs to be consistent to run!");
     */
    void runEfficient(unsigned int cycles);

    /**
     * Checks if the data valid the simulation is valid
     * @return bool; true if the data is consistent
     * * REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
     */
    bool isConsistent() const;

    /**
     * Resets the simulator to its startsstate
     * * REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
     */
    void reset();

private:
    // Initialization
    const Simulator* initCheck;

    // Simulation
    JObject* initialState;
    std::vector<Hub*> hubs;
    std::vector<VaccinationCenter*> centers;
    unsigned int daycount;
    SimulationData statistics;
    std::ostream* statisticsOutputStream;
    std::ostream* transportationOutputStream;
    std::ostream* vaccinationOutputStream;
};


#endif //VACCIN_SIMULATOR_SIMULATOR_H
