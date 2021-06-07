// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#include <iostream>
#include <cassert>
#include <vector>
#include <set>

#include "../src/parsing/XMLParser.h"
#include "../src/json/JObject.h"
#include "../src/json/JKeys.h"
#include "../src/json/JValue.h"
#include "../src/json/JArray.h"
#include "../src/entities/VaccinationCenter.h"
#include "TestUtils.h"
#include "../src/entities/Hub.h"
#include "../src/entities/SimulationData.h"
#include "../src/utilities/utils.h"
#include "../src/entities/Simulator.h"

#define EXPECT_TRUE(expression) assert(expression)
#define EXPECT_EQ(lhs, rhs) assert(lhs == rhs)
#define ITERATE(type, iteratable, name) for(type::iterator name = iteratable.begin(); name != iteratable.end(); name++)
#define ASSERT_TRUE assert

void expectErrors(const std::string &fileName, const ParseErrors &expectedErrors) {
    ParseErrors errors;
    EXPECT_TRUE(std::cout.good());
    JObject* parsed = XMLParser::parse(fileName, std::cout, errors);
    EXPECT_TRUE(std::cout.good());
    EXPECT_EQ(expectedErrors.size(), errors.size());
    for(int i = 0; i < (int) expectedErrors.size(); i++) {
        EXPECT_EQ(expectedErrors[i], errors[i]);
    }
    delete parsed;
}

int main() {
    Simulator simulator;
    // Setup Simulator
    // =============================================================================================
    // Create JSON structure
    JObject* json = new JObject();
    // Create centers
    JArray* jCenters = new JArray();
    jCenters->insertValue(new JValue(MockObjects::jCenter("Center 1", "Mainstreet", 130521, 2400)));
    jCenters->insertValue(new JValue(MockObjects::jCenter("Center 2", "Wallstreet", 1523040, 9100)));
    jCenters->insertValue(new JValue(MockObjects::jCenter("Center 3", "Route 66", 750140, 4500)));
    std::vector<std::string> centerNames;
    JValues jValueCenters = jCenters->getItems();
    ITERATE(JValues, jValueCenters, center) {
        centerNames.push_back((*center)->asJObject()->getValue(CENTER_NAME)->asString());
    }
    // Initialize Vaccines
    JArray* vaccines = new JArray();
    vaccines->insertValue(new JValue(MockObjects::jVaccine("Vaccine 1", 1000, 3, 500, 0, 0)));
    vaccines->insertValue(new JValue(MockObjects::jVaccine("Vaccine 2", 3000, 5, 600, 10, 0)));
    vaccines->insertValue(new JValue(MockObjects::jVaccine("Vaccine 3", 7000, 7, 480, 5, -30)));
    // Initialize Hubs
    JArray* hubs = new JArray();
    JObject* h = MockObjects::jHub(vaccines->getItems(), centerNames);
    hubs->insertValue(new JValue(h));
    // Load simulator object
    json->insertValue(SIMULATION_HUBS, new JValue(hubs));
    json->insertValue(SIMULATION_CENTERS, new JValue(jCenters));
    simulator.fromJSON(json);
    simulator.setInitialState(json);

    // Check if simulator object was created succesfully
    EXPECT_EQ(jCenters->getItems().size(), simulator.getCenters().size());
    EXPECT_EQ(hubs->getItems().size(), simulator.getHubs().size());
    EXPECT_TRUE(simulator.isConsistent());

    std::string fileName = "tests/presentation/out/ini_file_1_";
    simulator.exportSimulationIniFile(fileName, 0);

    simulator.run(50);

    fileName = "tests/presentation/out/ini_file_2_";
    simulator.exportSimulationIniFile(fileName, 0);

    simulator.run(50);

    fileName = "tests/presentation/out/ini_file_3_";
    simulator.exportSimulationIniFile(fileName, 0);


//    // Test Summary
//    simulator.reset();
//
//    simulator.runEfficient(40);
//
//    std::ofstream iniFile;
//    iniFile.open("tests/presentation/out/ini_file_1.ini");
//            ASSERT_TRUE(iniFile.is_open());
//    simulator.exportSimulationIniFile(iniFile);
//    iniFile.close();
//    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/ini_file_1.ini", "tests/presentation/expected/ini_file_1.ini"));
//
//    simulator.runEfficient(80);
//
//    iniFile.open("tests/presentation/out/ini_file_2.ini");
//            ASSERT_TRUE(iniFile.is_open());
//    simulator.exportSimulationIniFile(iniFile);
//    iniFile.close();
//    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/ini_file_2.ini", "tests/presentation/expected/ini_file_2.ini"));
    return 0;
}


