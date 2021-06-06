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
    // Setup Happyday Simulator
    // =============================================================================================
    // Create JSON structure
    Simulator happydaySimulator;
    JObject* jsonHappyday = new JObject();
    // Create centers
    JArray* jCentersHappyday = new JArray();
    jCentersHappyday->insertValue(new JValue(MockObjects::jCenter("Center 1", "Mainstreet", 154020, 2000)));
    jCentersHappyday->insertValue(new JValue(MockObjects::jCenter("Center 2", "Wallstreet", 785220, 2500)));
    jCentersHappyday->insertValue(new JValue(MockObjects::jCenter("Center 3", "Route 66", 946000, 5000)));
    jCentersHappyday->insertValue(new JValue(MockObjects::jCenter("Center 4", "Golden Bridge", 24300, 3400)));
    jCentersHappyday->insertValue(new JValue(MockObjects::jCenter("Center 5", "Champs-Elysee", 123456, 4000)));
    std::vector<std::string> centerNamesHappyday1;
    std::vector<std::string> centerNamesHappyday2;
    JValues jValueCentersHappyday = jCentersHappyday->getItems();
    int counter = 0;
    ITERATE(JValues, jValueCentersHappyday, center) {
        if(counter < 3)
            centerNamesHappyday1.push_back((*center)->asJObject()->getValue(CENTER_NAME)->asString());
        if(counter > 1)
            centerNamesHappyday2.push_back((*center)->asJObject()->getValue(CENTER_NAME)->asString());
        counter++;
    }
    // Initialize Vaccines
    JArray* vaccinesHappyday1 = new JArray();
    vaccinesHappyday1->insertValue(new JValue(MockObjects::jVaccine("Vaccine 1", 10000, 3, 500, 0, 0)));
    vaccinesHappyday1->insertValue(new JValue(MockObjects::jVaccine("Vaccine 2", 1000, 1, 600, 10, 0)));
    vaccinesHappyday1->insertValue(new JValue(MockObjects::jVaccine("Vaccine 3", 7000, 7, 480, 5, -30)));
    JArray* vaccinesHappyday2 = new JArray();
    vaccinesHappyday2->insertValue(new JValue(MockObjects::jVaccine("Vaccine 3", 7000, 7, 480, 5, -30)));
    vaccinesHappyday2->insertValue(new JValue(MockObjects::jVaccine("Vaccine 4", 2500, 5, 400, 0, 8)));
    vaccinesHappyday2->insertValue(new JValue(MockObjects::jVaccine("Vaccine 5", 3000, 2, 600, 0, 0)));
    // Initialize Hubs
    JArray* hubsHappyday = new JArray();
    hubsHappyday->insertValue(new JValue(MockObjects::jHub(vaccinesHappyday1->getItems(), centerNamesHappyday1)));
    hubsHappyday->insertValue(new JValue(MockObjects::jHub(vaccinesHappyday2->getItems(), centerNamesHappyday2)));
    // Load simulator object
    jsonHappyday->insertValue(SIMULATION_HUBS, new JValue(hubsHappyday));
    jsonHappyday->insertValue(SIMULATION_CENTERS, new JValue(jCentersHappyday));
    happydaySimulator.fromJSON(jsonHappyday);
    happydaySimulator.setInitialState(jsonHappyday);

    // Check if simulator object was created succesfully
    EXPECT_EQ(jCentersHappyday->getItems().size(), happydaySimulator.getCenters().size());
    EXPECT_EQ(hubsHappyday->getItems().size(), happydaySimulator.getHubs().size());
    EXPECT_TRUE(happydaySimulator.isConsistent());
    return 0;
}


