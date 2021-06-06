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
#include "../../src/json/JKeys.h"
#include "../../src/json/JObject.h"
#include "../../src/utilities/utils.h"
#include "../TestUtils.h"
#include "../../src/entities/SimulationData.h"
#include "../../src/entities/Simulator.h"

#include <gtest/gtest.h>

#define ITERATE(type, iteratable, name) for(type::iterator name = iteratable.begin(); name != iteratable.end(); name++)

class SimulationOutputTests: public ::testing::Test {
protected:
    Simulator simulator;

    void SetUp() {
        // Create JSON structure
        JObject* json = new JObject();
        // Create centers
        JArray* jCenters = new JArray();
        jCenters->insertValue(new JValue(MockObjects::jCenter("Center 1", "Mainstreet", 5043, 800)));
        jCenters->insertValue(new JValue(MockObjects::jCenter("Center 2", "Wallstreet", 8011, 600)));
        jCenters->insertValue(new JValue(MockObjects::jCenter("Center 3", "Route 66", 3021, 100)));
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
    }
};

/**
Tests the simple parsing format.
*/
TEST_F(SimulationOutputTests, SimpleOutput) {
    simulator.reset();

    ASSERT_TRUE(FileUtil::DirectoryExists("tests/presentation/out"));
    std::ofstream file;
    file.open("tests/presentation/out/simple_output_1.txt");
    ASSERT_TRUE(file.is_open());
    simulator.exportSimulationSummary(file);
    file.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/simple_output_1.txt", "tests/presentation/expected/simple_output_1.txt"));

    simulator.run(80);

    file.open("tests/presentation/out/simple_output_2.txt");
    ASSERT_TRUE(file.is_open());
    simulator.exportSimulationSummary(file);
    file.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/simple_output_2.txt", "tests/presentation/expected/simple_output_2.txt"));

}

/**
Tests the graphical parsing.
*/
TEST_F(SimulationOutputTests, GraphicalProgress) {
    simulator.reset();

    ASSERT_TRUE(FileUtil::DirectoryExists("tests/presentation/out"));
    std::ofstream file;
    file.open("tests/presentation/out/graphical_progress_1.txt");
    ASSERT_TRUE(file.is_open());
    simulator.exportSimulationProgress(file);
    file.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/graphical_progress_1.txt", "tests/presentation/expected/graphical_progress_1.txt"));

    simulator.run(80);

    file.open("tests/presentation/out/graphical_progress_2.txt");
    ASSERT_TRUE(file.is_open());
    simulator.exportSimulationProgress(file);
    file.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/graphical_progress_2.txt", "tests/presentation/expected/graphical_progress_2.txt"));
}

/**
 * Tests the ini file generation
 */
TEST_F(SimulationOutputTests, IniFileGeneration) {
    // Test Summary
    simulator.reset();

    std::ofstream iniFile;
    iniFile.open("tests/presentation/out/ini_file_1.ini");
    ASSERT_TRUE(iniFile.is_open());
    simulator.exportSimulationIniFile(iniFile);
    iniFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/ini_file_1.ini", "tests/presentation/expected/ini_file_1.ini"));

    simulator.run(80);

    iniFile.open("tests/presentation/out/ini_file_2.ini");
    ASSERT_TRUE(iniFile.is_open());
    simulator.exportSimulationIniFile(iniFile);
    iniFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/ini_file_2.ini", "tests/presentation/expected/ini_file_2.ini"));
}

/**
 * Test the parsing after the simulation ran for a specific amount of days
 */
TEST_F(SimulationOutputTests, HappyDay) {
    simulator.reset();

    simulator.run(100);

    ASSERT_TRUE(FileUtil::DirectoryExists("tests/presentation/out"));

    // Test Summary
    std::ofstream summaryFile;
    summaryFile.open("tests/presentation/out/simple_output_happyday_1.txt");
    ASSERT_TRUE(summaryFile.is_open());
    simulator.exportSimulationSummary(summaryFile);
    summaryFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/simple_output_happyday_1.txt", "tests/presentation/expected/simple_output_happyday_1.txt"));

    // Test Progress
    std::ofstream progressFile;
    progressFile.open("tests/presentation/out/graphical_progress_happyday_1.txt");
    ASSERT_TRUE(progressFile.is_open());
    simulator.exportSimulationProgress(progressFile);
    progressFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/graphical_progress_happyday_1.txt", "tests/presentation/expected/graphical_progress_happyday_1.txt"));

    // Test Ini
    std::ofstream iniFile;
    iniFile.open("tests/presentation/out/ini_file_happyday_1.ini");
    ASSERT_TRUE(iniFile.is_open());
    simulator.exportSimulationIniFile(iniFile);
    iniFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/ini_file_happyday_1.ini", "tests/presentation/expected/ini_file_happyday_1.ini"));

    simulator.run(50);

    // Test Summary
    summaryFile.open("tests/presentation/out/simple_output_happyday_2.txt");
    ASSERT_TRUE(summaryFile.is_open());
    simulator.exportSimulationSummary(summaryFile);
    summaryFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/simple_output_happyday_2.txt", "tests/presentation/expected/simple_output_happyday_2.txt"));

    // Test Progress
    progressFile.open("tests/presentation/out/graphical_progress_happyday_2.txt");
    ASSERT_TRUE(progressFile.is_open());
    simulator.exportSimulationProgress(progressFile);
    progressFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/graphical_progress_happyday_2.txt", "tests/presentation/expected/graphical_progress_happyday_2.txt"));

    // Test Ini
    iniFile.open("tests/presentation/out/ini_file_happyday_2.ini");
    ASSERT_TRUE(iniFile.is_open());
    simulator.exportSimulationIniFile(iniFile);
    iniFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/ini_file_happyday_2.ini", "tests/presentation/expected/ini_file_happyday_2.ini"));
}

TEST_F(SimulationOutputTests, Statistics) {
    // TODO: statistics test
}

