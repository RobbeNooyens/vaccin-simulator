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
    /**
     * Sets the statistics output stream to the given stream
     * @param stats: ostream*; stream to send statistics to
     * * REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
     */
    void setStatisticsStream(std::ostream* stats);
    /**
     * Sets the transportation output stream to the given stream
     * @param transport: ostream*; stream to send transportation notifications to
     * * REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
     */
    void setTransportationStream(std::ostream* transport);
    /**
     * Sets the vaccinations output stream to the given stream
     * @param vaccinations: ostream*; stream to send vaccination notifications to
     * * REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
     */
    void setVaccinationsStream(std::ostream* vaccinations);

    // IO
    /**
     * Imports the simulation from an XML file
     * @param fileName: name of the XML file with extension
     * * REQUIRE(properlyInitialized(), "Simulator object hasn't been properly initialized!");
     * * REQUIRE(!fileName.empty(), "Filename cannot be empty!");
     * * REQUIRE(errorStream.good(), "Errorstream is in an errorstate!");
     * * ENSURE(ParseUtil::isConsistent(getInitialState()), "Initialstate json should be consistent!");
     * * ENSURE(isConsistent(), "Simulator should be valid!");
     */
    void importSimulation(const std::string &fileName, std::ostream &errorStream);
    /**
     * Exports the simulation summary to a text file
     * @param out: name of the text file to write to
     * * REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
     * * REQUIRE(out.good(), "Output stream should be writeable!");
     * * REQUIRE(isConsistent(), "Simulator should be consistent!");
     * * ENSURE(out.good(), "Stream encountered an error during writing!");
     * * ENSURE(isConsistent(), "Simulator should be consistent after writing to output stream!");
     */
    void exportSimulationSummary(std::ostream &out) const;
    /**
     * Exports the simulation progress to a text file
     * @param out: name of the text file to write to
     * * REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
     * * REQUIRE(out.good(), "Output stream should be writeable!");
     * * REQUIRE(isConsistent(), "Simulator should be consistent!");
     * * ENSURE(out.good(), "Stream encountered an error during writing!");
     * * ENSURE(isConsistent(), "Simulator should be consistent after writing to output stream!");
     */
    void exportSimulationProgress(std::ostream &out) const;
    /**
     * Writes contents of the objects valid the simulation to an ini file
     * @param output: string; name of the file to write to
     * * REQUIRE(properlyInitialized(), "Object hasn't been initialized properly!");
     * * REQUIRE(StringUtil::contains(fileName, ".ini"), "File should be an ini file!");
     * * REQUIRE(!fileName.empty(), "Filename cannot be empty!");
     * * ENSURE(!file.is_open(), "File wasn't closed properly!");
     */
    void exportSimulationIniFile(std::string &output, unsigned int frames);
    /**
     *
     * @param json
     * * REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
     * * REQUIRE(json->contains(SIMULATION_CENTERS), StringUtil::concat("Can't load Simulator from JSON with missing field ", SIMULATION_CENTERS).c_str());
     * * REQUIRE(json->contains(SIMULATION_HUBS), StringUtil::concat("Can't load Simulator from JSON with missing field ", SIMULATION_HUBS).c_str());
     * * REQUIRE(ParseUtil::isConsistent(json), "Simulation JSON should be consistent!");
     * * ENSURE(!getHubs().empty(), "Couldn't load hubs properly!");
     * * ENSURE(!getCenters().empty(), "Couldn't load centers properly!");
     * * ENSURE(isConsistent(), "Simulation should be consistent after reading json!");
     */
    void fromJSON(JObject* json);

    // Controls
    /**
     * Runs the simulation for a specific amount of times
     * @param cycles: the amount of times the simulation should run
     * * REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
     * * REQUIRE(cycles != 0, "Cycles cannot be 0!");
     * * REQUIRE(isConsistent(), "Simulation needs to be consistent to run!");
     * * ENSURE(getDayCount() == oldDaycount + cycles, "Simulator didn't succesfully finish the right amount of cycles!");
     * * ENSURE(isConsistent(), "Simulation needs to be consistent after running!");
     */
    void run(const unsigned int cycles, bool smartDistribution = false);
    /**
     * Runs the simulation for a specific amount of times with smart distribution
     * @param startFromDay: int; the day to start from
     * * REQUIRE(properlyInitialized(), "Object hasn't been initialized properly!");
     * * REQUIRE(cycles != 0, "Cycles cannot be 0!");
     * * REQUIRE(isConsistent(), "Simulation needs to be consistent to run!");
     * * ENSURE(getDayCount() == oldDaycount + cycles, "Simulator didn't succesfully finish the right amount of cycles!");
     * * ENSURE(isConsistent(), "Simulation needs to be consistent after running!");
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
     * * ENSURE(isConsistent(), "Simulator should be consistent after reset!");
     */
    void reset();
    /**
     * @return JObject; Initial state as json
     * * REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
     */
    JObject* getInitialState();
    /**
     * @return unsigned int; current day count
     * * REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
     */
    unsigned int getDayCount();
    /**
     * Sets the initial json to the given json object
     * @param json: JObject*; object to set as initial state
     * * REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
     */
    void setInitialState(JObject* json);
    /**
     * @return VaccinationCenters&; centers stored in the simulation
     * * REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
     */
    VaccinationCenters& getCenters();
    /**
     * @return std::vector<Hub*>; hubs stored in the simulation
     * * REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
     */
    std::vector<Hub*> getHubs();

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
