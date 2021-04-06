// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include <iostream>
#include <string>

#include "Simulator.h"

#include "json/JObject.h"
#include "json/JArray.h"
#include "json/JValue.h"
#include "utils.h"

int main() {
    Simulator simulator = Simulator();
    simulator.importSimulation("io/input.xml");
    simulator.run(1);
    simulator.exportSimulation("io/output.txt");
    return 0;
}
