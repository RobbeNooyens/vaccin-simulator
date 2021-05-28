// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛
#include "../../src/entities/VaccinationCenter.h"
#include "../../src/entities/Hub.h"
#include "../../src/json/JArray.h"
#include "../../src/json/JValue.h"
#include "../../src/json/JObject.h"
#include "../../src/utilities/utils.h"
#include "../TestUtils.h"

#include <gtest/gtest.h>

#define ITERATE(type, iteratable, name) for(type::iterator name = iteratable.begin(); name != iteratable.end(); name++)

class SimulationOutputTests: public ::testing::Test {
protected:
    Hub hub;

    void SetUp() {
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
        hub.fromJSON(h, centers);
        // TODO: delete json without segmentation fault
//        ITERATE(VaccinationCenters, centers, center)
//            delete *center;
        // delete json;
    }


};

/**
Tests the simple parsing format.
*/
TEST_F(SimulationOutputTests, SimpleOutput) {
    ASSERT_TRUE(FileUtil::DirectoryExists("tests/presentation/out"));
    std::ofstream file;
    file.open("tests/presentation/out/simple_output.txt");
    ASSERT_TRUE(file.is_open());
    hub.toSummaryStream(file);
    file << std::endl;
    VaccinationCenters centers = hub.getVaccinationCenters();
    ITERATE(VaccinationCenters, centers, center)
        (*center)->toSummaryStream(file);
    file.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/simple_output.txt", "tests/presentation/expected/simple_output.txt"));

}

/**
Tests the graphical parsing.
*/
TEST_F(SimulationOutputTests, GraphicalProgress) {
    ASSERT_TRUE(FileUtil::DirectoryExists("tests/presentation/out"));
    std::ofstream file;
    file.open("tests/presentation/out/graphical_progress.txt");
    ASSERT_TRUE(file.is_open());
    hub.toProgressStream(file);
    file.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/graphical_progress.txt", "tests/presentation/expected/graphical_progress.txt"));
}

/**
 * Test the parsing after the simulation ran for a specific amount of days
 */
TEST_F(SimulationOutputTests, HappyDay) {
    for(int day = 1; day <= 10; day++)
        hub.simulateDay(day, NULL, NULL);
    ASSERT_TRUE(FileUtil::DirectoryExists("tests/presentation/out"));

    // Test Summary
    std::ofstream summaryFile;
    summaryFile.open("tests/presentation/out/simple_output_after_simulation.txt");
    ASSERT_TRUE(summaryFile.is_open());
    hub.toSummaryStream(summaryFile);
    summaryFile << std::endl;
    VaccinationCenters centers = hub.getVaccinationCenters();
    ITERATE(VaccinationCenters, centers, center)
        (*center)->toSummaryStream(summaryFile);
    summaryFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/simple_output_after_simulation.txt", "tests/presentation/expected/simple_output_after_simulation.txt"));

    // Test Progress
    std::ofstream progressFile;
    progressFile.open("tests/presentation/out/graphical_progress_after_simulation.txt");
    ASSERT_TRUE(progressFile.is_open());
    hub.toProgressStream(progressFile);
    progressFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/graphical_progress_after_simulation.txt", "tests/presentation/expected/graphical_progress_after_simulation.txt"));
}