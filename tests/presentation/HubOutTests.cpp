// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛
#include "../../entities/VaccinationCenter.h"
#include "../../entities/Hub.h"
#include "../../json/JArray.h"
#include "../../json/JValue.h"
#include "../../json/JObject.h"
#include "../../utils.h"
#include "../TestUtils.h"

#include <gtest/gtest.h>


class HubOutTests: public ::testing::Test {
protected:
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.
    Hub hub;
};

/**
Tests the simple output format.
*/
TEST_F(HubOutTests, SimpleOutput) {
    JObject* json = new JObject();
    // Initialize Hub
    JObject* h = MockObjects::jHub(100, 6, 40);
    json->insertValue("hub", new JValue(h));

    // Initialize centra
    JArray* centra = new JArray();
    json->insertValue("centra", new JValue(centra));

    JObject* center1 = MockObjects::jCenter("Center 1", "Mainstreet", 5043, 200);
    JObject* center2 = MockObjects::jCenter("Center 2", "Wallstreet", 8011, 600);
    JObject* center3 = MockObjects::jCenter("Center 3", "Route 66", 3021, 100);
    centra->insertValue(new JValue(center1));
    centra->insertValue(new JValue(center2));
    centra->insertValue(new JValue(center3));

    hub.fromJSON(json);

    ASSERT_TRUE(FileUtil::DirectoryExists("tests/presentation/out"));

    std::ofstream file;
    file.open("tests/presentation/out/simple_output.txt");
    ASSERT_TRUE(file.is_open());
    hub.toSummaryStream(file);
    file.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/simple_output.txt", "tests/presentation/expected/simple_output.txt"));

    delete json;
}

/**
Tests the graphical output.
*/
TEST_F(HubOutTests, GraphicalProgress) {
    JObject* json = new JObject();
    // Initialize Hub
    JObject* h = MockObjects::jHub(100, 6, 40);
    json->insertValue("hub", new JValue(h));

    // Initialize centra
    JArray* centra = new JArray();
    json->insertValue("centra", new JValue(centra));

    JObject* center1 = MockObjects::jCenter("Center 1", "Mainstreet", 5043, 200);
    JObject* center2 = MockObjects::jCenter("Center 2", "Wallstreet", 8011, 600);
    JObject* center3 = MockObjects::jCenter("Center 3", "Route 66", 3021, 100);
    centra->insertValue(new JValue(center1));
    centra->insertValue(new JValue(center2));
    centra->insertValue(new JValue(center3));

    hub.fromJSON(json);

    ASSERT_TRUE(FileUtil::DirectoryExists("tests/presentation/out"));

    std::ofstream file;
    file.open("tests/presentation/out/graphical_progress.txt");
    ASSERT_TRUE(file.is_open());
    hub.toProgressStream(file);
    file.close();
    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/graphical_progress.txt", "tests/presentation/expected/graphical_progress.txt"));

    delete json;
}