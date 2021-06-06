// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#ifndef VACCIN_SIMULATOR_SIMULATOR_H
#define VACCIN_SIMULATOR_SIMULATOR_H

#include "entities/Hub.h"
#include "io/XMLParser.h"
#include <string>

typedef std::vector<Hub> Hubs;
typedef std::vector<VaccinationCenter> Centers;

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

    // Run
    /**
     * Runs the simulation for a specific amount of times
     * @param cycles: the amount of times the simulation should run
     * REQUIRE(properlyInitialized(), "Object hasn't been initialized properly!");
     * REQUIRE(cycles != 0, "Cycles cannot be 0!");
     * ENSURE(daycount == oldDaycount + cycles, "Simulator didn't succesfully finish the right amount of cycles!");
     */
    void run();

    /**
     * simulates a day of the simulation
     * @param day: specific day of the simulation procces
     * REQUIRE(properlyInitialized(), "Object hasn't been initialized properly!");
     * REQUIRE(day < 0, "day cannot be negative!");
     * REQUIRE(day+1 > cycles, "day cannot be bigger than total days defined in the simulation!");
     * ENSURE(daycount == oldDaycount + cycles, "Simulator didn't succesfully finish the right amount of cycles!");
     */
    void simulateDay(const unsigned int day);

private:
    // Initialization
    const Simulator* initCheck;

    // Days defined
    unsigned int cycles;

    // defined hubs
    std::vector<Hub*> hubs;

    // map from center name to index in "centers" vector
    map<std::string, int> center2idx;

    // defined centers
    std::vector<VaccinationCenter*> centers;

    // defined vaccins in all hubs
    std::vector<Vaccine*> vaccins;

    // parser
    XMLParser xmlParser;

    // dynamic daycount, gives what day we are currently running the simulation on
    unsigned int daycount;

    // plan for total days for transporting vaccins between hubs en centra
    Planning plan;
};


#endif //VACCIN_SIMULATOR_SIMULATOR_H
