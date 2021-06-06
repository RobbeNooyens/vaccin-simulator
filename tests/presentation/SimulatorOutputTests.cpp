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
    Simulator happydaySimulator;

    void SetUp() {
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


        // Setup Happyday Simulator
        // =============================================================================================
        // Create JSON structure
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

TEST_F(SimulationOutputTests, Statistics) {
    simulator.reset();

    std::ofstream statisticsFile;
    statisticsFile.open("tests/presentation/out/statistics_1.txt");
    ASSERT_TRUE(statisticsFile.is_open());
    simulator.setStatisticsStream(&statisticsFile);
    simulator.run(1);
    statisticsFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/statistics_1.txt", "tests/presentation/expected/statistics_1.txt"));


    statisticsFile.open("tests/presentation/out/statistics_2.txt");
    ASSERT_TRUE(statisticsFile.is_open());
    simulator.setStatisticsStream(&statisticsFile);
    simulator.run(100);
    statisticsFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/statistics_2.txt", "tests/presentation/expected/statistics_2.txt"));

    simulator.setStatisticsStream(NULL);
}

/**
 * Tests the vaccine transportation during the simulation
 */
TEST_F(SimulationOutputTests, Transportation) {
    simulator.reset();

    std::ofstream transportationFile;
    transportationFile.open("tests/presentation/out/transportation_1.txt");
    ASSERT_TRUE(transportationFile.is_open());
    simulator.setTransportationStream(&transportationFile);

    simulator.run(10);

    transportationFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/transportation_1.txt", "tests/presentation/expected/transportation_1.txt"));

    transportationFile.open("tests/presentation/out/transportation_2.txt");
    ASSERT_TRUE(transportationFile.is_open());
    simulator.setTransportationStream(&transportationFile);

    simulator.run(50);

    transportationFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/transportation_2.txt", "tests/presentation/expected/transportation_2.txt"));

    simulator.setTransportationStream(NULL);
}

/**
 * Tests the vaccinations during the simulation
 */
TEST_F(SimulationOutputTests, Vaccinations) {
    simulator.reset();

    std::ofstream vaccinationsFile;
    vaccinationsFile.open("tests/presentation/out/vaccinations_1.txt");
    ASSERT_TRUE(vaccinationsFile.is_open());
    simulator.setVaccinationsStream(&vaccinationsFile);

    simulator.run(30);

    vaccinationsFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/vaccinations_1.txt", "tests/presentation/expected/vaccinations_1.txt"));

    vaccinationsFile.open("tests/presentation/out/vaccinations_2.txt");
    ASSERT_TRUE(vaccinationsFile.is_open());
    simulator.setVaccinationsStream(&vaccinationsFile);

    simulator.run(60);

    vaccinationsFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/vaccinations_2.txt", "tests/presentation/expected/vaccinations_2.txt"));

    simulator.setVaccinationsStream(NULL);
}

/**
 * Test the parsing after the simulation ran for a specific amount of days
 */
TEST_F(SimulationOutputTests, HappyDay) {
    happydaySimulator.reset();

    // Setup Statistics
    std::ofstream statisticsFile;
    statisticsFile.open("tests/presentation/out/statistics_happyday_1.txt");
    ASSERT_TRUE(statisticsFile.is_open());
    happydaySimulator.setStatisticsStream(&statisticsFile);

    // Setup Transportation
    std::ofstream transportationFile;
    transportationFile.open("tests/presentation/out/transportation_happyday.txt");
    ASSERT_TRUE(transportationFile.is_open());
    happydaySimulator.setTransportationStream(&transportationFile);

    // Setup Vaccinations
    std::ofstream vaccinationsFile;
    vaccinationsFile.open("tests/presentation/out/vaccinations_happyday.txt");
    ASSERT_TRUE(vaccinationsFile.is_open());
    happydaySimulator.setVaccinationsStream(&vaccinationsFile);

    happydaySimulator.run(100);

    ASSERT_TRUE(FileUtil::DirectoryExists("tests/presentation/out"));

    // Test Summary
    std::ofstream summaryFile;
    summaryFile.open("tests/presentation/out/simple_output_happyday_1.txt");
    ASSERT_TRUE(summaryFile.is_open());
    happydaySimulator.exportSimulationSummary(summaryFile);
    summaryFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/simple_output_happyday_1.txt",
                                      "tests/presentation/expected/simple_output_happyday_1.txt"));

    // Test Progress
    std::ofstream progressFile;
    progressFile.open("tests/presentation/out/graphical_progress_happyday_1.txt");
    ASSERT_TRUE(progressFile.is_open());
    happydaySimulator.exportSimulationProgress(progressFile);
    progressFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/graphical_progress_happyday_1.txt",
                                      "tests/presentation/expected/graphical_progress_happyday_1.txt"));

    // Test Ini
    std::ofstream iniFile;
    iniFile.open("tests/presentation/out/ini_file_happyday_1.ini");
    ASSERT_TRUE(iniFile.is_open());
    happydaySimulator.exportSimulationIniFile(iniFile);
    iniFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/ini_file_happyday_1.ini",
                                      "tests/presentation/expected/ini_file_happyday_1.ini"));


    // Test Statistics
    statisticsFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/statistics_happyday_1.txt", "tests/presentation/expected/statistics_happyday_1.txt"));

    // Second part

    // Setup Statistics
    statisticsFile.open("tests/presentation/out/statistics_happyday_2.txt");
    ASSERT_TRUE(statisticsFile.is_open());

    happydaySimulator.run(50);

    // Test Summary
    summaryFile.open("tests/presentation/out/simple_output_happyday_2.txt");
    ASSERT_TRUE(summaryFile.is_open());
    happydaySimulator.exportSimulationSummary(summaryFile);
    summaryFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/simple_output_happyday_2.txt",
                                      "tests/presentation/expected/simple_output_happyday_2.txt"));

    // Test Progress
    progressFile.open("tests/presentation/out/graphical_progress_happyday_2.txt");
    ASSERT_TRUE(progressFile.is_open());
    happydaySimulator.exportSimulationProgress(progressFile);
    progressFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/graphical_progress_happyday_2.txt",
                                      "tests/presentation/expected/graphical_progress_happyday_2.txt"));

    // Test Ini
    iniFile.open("tests/presentation/out/ini_file_happyday_2.ini");
    ASSERT_TRUE(iniFile.is_open());
    happydaySimulator.exportSimulationIniFile(iniFile);
    iniFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/ini_file_happyday_2.ini",
                                      "tests/presentation/expected/ini_file_happyday_2.ini"));

    // Test Statistics
    statisticsFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/statistics_happyday_2.txt", "tests/presentation/expected/statistics_happyday_2.txt"));

    // Test Transportation
    transportationFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/transportation_happyday.txt", "tests/presentation/expected/transportation_happyday.txt"));

    // Test Vaccinations
    vaccinationsFile.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/vaccinations_happyday.txt", "tests/presentation/expected/vaccinations_happyday.txt"));

    happydaySimulator.setStatisticsStream(NULL);
}

