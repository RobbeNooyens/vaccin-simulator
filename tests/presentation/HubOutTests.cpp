// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛
#include "../../entities/VaccinationCenter.h"
#include "../../entities/Hub.h"
#include "../../json/JArray.h"
#include "../../json/JValue.h"
#include "../../json/JObject.h"
#include "../../utils.h"

#include <fstream>
#include <gtest/gtest.h>


class HubOutTests: public ::testing::Test {
protected:
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.
    Hub hub;
};

/**
Tests the output.
*/
TEST_F(HubOutTests, Output) {
    JObject* json = new JObject();
    // Initialize Hub
    JObject* h = new JObject();
    json->insertValue("hub", new JValue(h));
    h->insertValue("levering", new JValue(100));
    h->insertValue("interval", new JValue(6));
    h->insertValue("transport", new JValue(40));
    // Initialize centra
    JArray* centra = new JArray();
    json->insertValue("centra", new JValue(centra));

    JObject* center1 = new JObject();
    center1->insertValue("adres", new JValue("Mainstreet"));
    center1->insertValue("capaciteit", new JValue("200"));
    center1->insertValue("inwoners", new JValue("5043"));
    center1->insertValue("naam", new JValue("Center 1"));
    centra->insertValue(new JValue(center1));

    JObject* center2 = new JObject();
    center2->insertValue("adres", new JValue("Wallstreet"));
    center2->insertValue("capaciteit", new JValue("600"));
    center2->insertValue("inwoners", new JValue("8011"));
    center2->insertValue("naam", new JValue("Center 2"));
    centra->insertValue(new JValue(center2));

    JObject* center3 = new JObject();
    center3->insertValue("adres", new JValue("Route 66"));
    center3->insertValue("capaciteit", new JValue("100"));
    center3->insertValue("inwoners", new JValue("3021"));
    center3->insertValue("naam", new JValue("Center 3"));
    centra->insertValue(new JValue(center3));

    hub.fromJSON(json);

    ASSERT_TRUE(FileUtil::DirectoryExists("tests/presentation/out"));

    std::ofstream file;
    file.open("tests/presentation/out/hub_output.txt");
    ASSERT_TRUE(file.is_open());
    hub.toStream(file);
    file.close();
//    EXPECT_TRUE(FileUtil::FileCompare("tests/presentation/out/hub_output.txt", "tests/presentation/expected/hub_output.txt"));

    delete json;
}
