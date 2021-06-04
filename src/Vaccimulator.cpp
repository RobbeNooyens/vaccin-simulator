// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#include <iostream>
#include <cassert>

#include "entities/Simulator.h"

#include "json/JObject.h"
#include "json/JArray.h"
#include "json/JValue.h"

#include "../tests/TestUtils.h"
#include "utilities/utils.h"

#define ITERATE(type, iteratable, name) for(type::iterator name = iteratable.begin(); name != iteratable.end(); name++)
#define ASSERT(lhs, rhs) assert(lhs == rhs)

using namespace TimeUtil;
using namespace StringUtil;

int main() {
    Simulator simulator = Simulator();
    // TODO: program argument
    simulator.importSimulation("io/xml_files/time_test.xml", std::cerr);
//    simulator.runEfficient(30);
    simulator.run(5, std::cout);
    std::string file = concat("io/output/Simulation_", concat(getCurrentDateTime("%Y-%m-%d_%H-%M-%S"), ".txt"));
//    simulator.exportSimulationSummary(file);
    simulator.exportSimulationProgress(file);
//    simulator.exportSimulationIniFile("parsing/ini_files/generated.ini");
    return 0;
}
