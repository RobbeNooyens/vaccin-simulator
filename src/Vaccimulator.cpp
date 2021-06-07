// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#include <iostream>
#include <cassert>
#include <fstream>

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
    std::string fileName = concat("io/output/Simulation_", concat(getCurrentDateTime("%Y-%m-%d_%H-%M-%S"), ".txt"));
    std::ofstream file;
    file.open(fileName.c_str());

    Simulator simulator = Simulator();
    simulator.setStatisticsStream(&file);
    simulator.setTransportationStream(&file);
    simulator.setVaccinationsStream(&file);

    simulator.importSimulation("io/xml_files/input.xml", std::cerr);
//    simulator.runEfficient(30);
    simulator.run(30, false);
//    simulator.exportSimulationSummary(file);
    simulator.exportSimulationProgress(file);
//    simulator.exportSimulationIniFile("parsing/ini_files/generated.ini");
    simulator.run(30, false);
//    simulator.exportSimulationSummary(file);
    simulator.exportSimulationProgress(file);
    return 0;
}
