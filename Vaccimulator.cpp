// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include <iostream>
#include <cassert>

#include "entities/Simulator.h"

#include "json/JObject.h"
#include "json/JArray.h"
#include "json/JValue.h"

#include "tests/TestUtils.h"
#include "utils.h"

#define ITERATE(type, iteratable, name) for(type::iterator name = iteratable.begin(); name != iteratable.end(); name++)
#define ASSERT(lhs, rhs) assert(lhs == rhs)

using namespace TimeUtil;
using namespace StringUtil;

int main() {
    Simulator simulator = Simulator();
    simulator.importSimulation("io/input2.xml");
    simulator.runEfficient(30);
    simulator.run(5);
    std::string file = concat("io/output/Simulation_", concat(getCurrentDateTime("%Y-%m-%d_%H-%M-%S"), ".txt"));
    simulator.exportSimulationSummary(file);
    simulator.exportSimulationProgress(file);
    simulator.exportSimulationIniFile("io/ini_files/generated.ini");
    return 0;
}
