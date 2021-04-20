// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#ifndef VACCIN_SIMULATOR_SIMULATOR_H
#define VACCIN_SIMULATOR_SIMULATOR_H

#include "io/XMLParser.h"
#include <string>
#include <vector>

class Hub;
class VaccinationCenter;
class JObject;

typedef std::vector<Hub> Hubs;
typedef std::vector<VaccinationCenter> VaccinationCenters;

class Simulator {
public:
    // Constructor
    /**
     * ENSURE(properlyInitialized(), "Object hasn't been initialized properly!");
     */
    Simulator();

    // Initialization
    bool properlyInitialized() const;

    // IO
    /**
     * Imports the simulation from an XML file
     * @param fileName: name of the XML file with extension
     * REQUIRE(properlyInitialized(), "Object hasn't been properly initialized!");
     * REQUIRE(!fileName.empty(), "Filename cannot be empty!");
     */
    void importSimulation(const std::string& fileName);
    /**
     * Exports the simulation to a text file
     * @param fileName: name of the text file to write to
     * REQUIRE(properlyInitialized(), "Object hasn't been initialized properly!");
     * REQUIRE(!fileName.empty(), "Filename cannot be empty!");
     * ENSURE(!file.is_open(), "File wasn't closed properly!");
     */
    void exportSimulation(const std::string& fileName) const;
    void fromJSON(JObject* json);

    // Controls
    /**
     * Runs the simulation for a specific amount of times
     * @param cycles: the amount of times the simulation should run
     * REQUIRE(properlyInitialized(), "Object hasn't been initialized properly!");
     * REQUIRE(cycles != 0, "Cycles cannot be 0!");
     * ENSURE(daycount == oldDaycount + cycles, "Simulator didn't succesfully finish the right amount of cycles!");
     */
    void run(unsigned int cycles);
    // TODO: implement restart and clear functions
//    void reset();
//    void clear();

private:
    // Initialization
    const Simulator* initCheck;

    // Simulation
    Hubs hubs;
    VaccinationCenters centers;
    XMLParser xmlParser;
    unsigned int daycount;
};


#endif //VACCIN_SIMULATOR_SIMULATOR_H
