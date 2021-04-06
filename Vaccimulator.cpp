// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include <iostream>

#include "Simulator.h"

#include "json/JObject.h"
#include "json/JArray.h"
#include "json/JValue.h"

int main() {
    Simulator simulator = Simulator();
    simulator.importSimulation("io/input.xml");
    simulator.run(20);
    simulator.exportSimulation("io/output.txt");
    return 0;
}
