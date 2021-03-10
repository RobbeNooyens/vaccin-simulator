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

Simulator::Simulator(): daycount(0) {
}

void Simulator::importSimulation(const std::string& fileName) {
    assert(!fileName.empty() && stringutil::contains(fileName, ".xml"));
}

void Simulator::exportSimulation(const std::string& fileName) const {
    assert(!fileName.empty() && stringutil::contains(fileName, ".txt"));
    std::ofstream file;
    file.open(fileName.c_str());
    assert(file.is_open());
    hub.toStream(file);
    file.close();
    assert(!file.is_open());
}

void Simulator::run(const unsigned int cycles) {
    unsigned int lastDay = daycount + cycles;
    for(; daycount < lastDay; daycount++){
        hub.simulateDay(daycount);
    }
}

bool Simulator::properlyInitialized() const {
    return initCheck == this;
}
