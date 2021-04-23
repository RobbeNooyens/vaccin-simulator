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

#include "utils.h"

using namespace StringUtil;
using namespace TimeUtil;

int main() {
    Simulator simulator = Simulator();
    simulator.importSimulation("io/input2.xml");
    simulator.run(30);
    std::string file = concat("io/output/Simulation_", concat(getCurrentDateTime("%Y-%m-%d_%H-%M-%S"), ".txt"));
    simulator.exportSimulationSummary(file);
    simulator.exportSimulationProgress(file);
    return 0;
}
