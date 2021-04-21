// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include <iostream>

#include "entities/Simulator.h"

#include "json/JObject.h"
#include "json/JArray.h"
#include "json/JValue.h"

int main() {
    Simulator simulator = Simulator();
    simulator.importSimulation("io/input2.xml");
    simulator.run(20);
    simulator.exportSimulationSummary("io/output.txt");
    simulator.exportSimulationProgress("io/output2.txt");
    return 0;
}
