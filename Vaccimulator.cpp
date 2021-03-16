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
    Simulator simulator = Simulator();
    simulator.importSimulation("io/input.xml");
    simulator.run(1);
    simulator.exportSimulation("io/output.txt");
    return 0;
}
