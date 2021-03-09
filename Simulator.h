// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#ifndef VACCIN_SIMULATOR_SIMULATOR_H
#define VACCIN_SIMULATOR_SIMULATOR_H

#include "entities/Hub.h"
#include <string>

class Simulator {
public:
    // Initialization
    const Simulator* initCheck;

    // Simulation
    Hub hub;
    unsigned int daycount;

    // Constructor
    Simulator();
    bool properlyInitialized() const;

    // IO
    void importSimulation(const std::string& fileName);
    void exportSimulation(const std::string& fileName) const;

    // Run
    void run(unsigned int cycles);

//    void reset();
};


#endif //VACCIN_SIMULATOR_SIMULATOR_H
