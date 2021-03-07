//
// Created by robbe on 25/02/2021.
//

#include <fstream>
#include "Simulator.h"

Simulator::Simulator(): daycount(0) {
}

void Simulator::importSimulation(const std::string& fileName) {

}

void Simulator::exportSimulation(const std::string& fileName) {
    std::ofstream file;
    file.open("output.txt");
    hub.toStream(file);
    file.close();
}

void Simulator::run(const unsigned int cycles) {

}

void Simulator::cycle() {

}
