//
// Created by robbe on 25/02/2021.
//

#include <fstream>
#include <cassert>
#include "Simulator.h"
#include "utils.h"

Simulator::Simulator(): daycount(0) {
}

void Simulator::importSimulation(const std::string& fileName) {

}

void Simulator::exportSimulation(const std::string& fileName) {
    assert(!fileName.empty() && stringContains(fileName, ".xml"));
    std::ofstream file;
    file.open("output.txt");
    assert(file.is_open());
    hub.toStream(file);
    file.close();
    assert(!file.is_open());
}

void Simulator::run(const unsigned int cycles) {

}

void Simulator::cycle() {

}
