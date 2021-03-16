// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#ifndef VACCIN_SIMULATOR_SIMULATOR_H
#define VACCIN_SIMULATOR_SIMULATOR_H

#include "entities/Hub.h"
#include "io/XMLParser.h"
#include <string>

class Simulator {
public:
    // Initialization
    const Simulator* initCheck;

    // Simulation
    Hub hub;
    XMLParser xmlParser;
    unsigned int daycount;

    // Constructor
    /**
     * ENSURE(properlyInitialized(), "Object hasn't been initialized properly!");
     */
    Simulator();
    bool properlyInitialized() const;

    // IO
    /**
     * Imports the simulation from an XML file
     * @param fileName: name of the XML file with extension
     * REQUIRE(properlyInitialized(), "Object hasn't been properly initialized!");
     * REQUIRE(!fileName.empty(), "Filename cannot be empty!");
     * REQUIRE(stringutil::contains(fileName, ".xml"), "File should be an XML file!");
     */
    void importSimulation(const std::string& fileName);
    /**
     * Exports the simulation to a text file
     * @param fileName: name of the text file to write to
     * REQUIRE(properlyInitialized(), "Object hasn't been initialized properly!");
     * REQUIRE(!fileName.empty(), "Filename cannot be empty!");
     * REQUIRE(stringutil::contains(fileName, ".txt"), "File should be a text file!");
     */
    void exportSimulation(const std::string& fileName) const;

    // Run
    /**
     * Runs the simulation for a specific amount of times
     * @param cycles: the amount of times the simulation should run
     * REQUIRE(properlyInitialized(), "Object hasn't been initialized properly!");
     * REQUIRE(cycles != 0, "Cycles cannot be 0!");
     */
    void run(unsigned int cycles);
};


#endif //VACCIN_SIMULATOR_SIMULATOR_H
