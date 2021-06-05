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

#define EXPECT_TRUE(expression) assert(expression)
#define EXPECT_EQ(lhs, rhs) assert(lhs == rhs)
#define ITERATE(type, iteratable, name) for(type::iterator name = iteratable.begin(); name != iteratable.end(); name++)

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
    // Create JSON structure
    JObject* json = new JObject();
    // Create centers
    JArray* jCenters = new JArray();
    json->insertValue("jCenters", new JValue(jCenters));
    JObject* center1 = MockObjects::jCenter("Center 1", "Mainstreet", 5043, 200);
    JObject* center2 = MockObjects::jCenter("Center 2", "Wallstreet", 8011, 600);
    JObject* center3 = MockObjects::jCenter("Center 3", "Route 66", 3021, 100);
    jCenters->insertValue(new JValue(center1));
    jCenters->insertValue(new JValue(center2));
    jCenters->insertValue(new JValue(center3));
    VaccinationCenters centers;
    std::vector<std::string> centerNames;
    JValues jValueCenters = jCenters->getItems();
    ITERATE(JValues, jValueCenters, center) {
        VaccinationCenter* c = new VaccinationCenter();
        c->fromJSON((*center)->asJObject());
        centers.push_back(c);
        centerNames.push_back(c->getName());
    }
    // Initialize Hubs
    JArray* hubs = new JArray();
    JObject* h = MockObjects::jHub(100, 6, 40, centerNames);
    hubs->insertValue(new JValue(h));
    // Load hub
    Hub hub;
    SimulationData statistics;
    hub.fromJSON(h, centers);
    delete json;
    for(int day = 1; day <= 10; day++)
        hub.simulateDay(day, statistics);
    EXPECT_TRUE(FileUtil::DirectoryExists("tests/presentation/out"));
    return 0;
}


