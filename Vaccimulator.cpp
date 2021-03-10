// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include <iostream>
#include <string>

#include "Simulator.h"

int main() {
    std::cout << "Vaccin Simulator" << std::endl;
    Simulator simulator = Simulator();
    simulator.importSimulation("input.xml");
    simulator.run(10);
    simulator.exportSimulation("output.txt");
    return 0;
}
