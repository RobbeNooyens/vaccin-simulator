// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include <fstream>
#include <cassert>
#include "Simulator.h"
#include "utils.h"
#include "DesignByContract.h"
#include <cassert>


Simulator::Simulator(): daycount(0) {
    ENSURE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
}

void Simulator::importSimulation(const std::string& fileName) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been properly initialized!");
    REQUIRE(!fileName.empty(), "Filename cannot be empty!");
    REQUIRE(stringutil::contains(fileName, ".xml"), "File should be an XML file!");
}

void Simulator::exportSimulation(const std::string& fileName) const {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(!fileName.empty(), "Filename cannot be empty!");
    REQUIRE(stringutil::contains(fileName, ".txt"), "File should be a text file!");
    std::ofstream file;
    file.open(fileName.c_str());
    assert(file.is_open());
    hub.toStream(file);
    file.close();
    assert(!file.is_open());
}

void Simulator::run(const unsigned int cycles) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(cycles != 0, "Cycles cannot be 0!");
    unsigned int lastDay = daycount + cycles;
    for(; daycount < lastDay; daycount++){
        hub.simulateDay(daycount);
    }
}

bool Simulator::properlyInitialized() const {
    return initCheck == this;
}
