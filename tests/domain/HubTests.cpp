// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛
#include <gtest/gtest.h>
#include "../../entities/VaccinationCenter.h"
#include "../../entities/Hub.h"
#include "../../entities/Vaccine.h"
#include "../TestUtils.h"
#include "../../json/JValue.h"
#include "../../json/JObject.h"

#define ITERATE(type, iteratable, name) for(type::iterator name = iteratable.begin(); name != iteratable.end(); name++)


class HubTests: public ::testing::Test {
protected:
    VaccinationCenters vaccinationCenters;
    std::vector<std::string> centerNames;
    JValues vaccines;

    virtual void SetUp() {
        // Setup VaccinationCenters
        std::string name1 = "Test 1", address1 = "Teststreet 1";
        unsigned int capacity1 = 100, inhabitants1 = 1000;
        VaccinationCenter* center1 = new VaccinationCenter(name1, address1, inhabitants1, capacity1);
        vaccinationCenters.push_back(center1);
        centerNames.push_back(name1);
        std::string name2 = "Test 2", address2 = "Teststreet 2";
        unsigned int capacity2 = 200, inhabitants2 = 2000;
        VaccinationCenter* center2 = new VaccinationCenter(name2, address2, inhabitants2, capacity2);
        vaccinationCenters.push_back(center2);
        centerNames.push_back(name2);
        std::string name3 = "Test 3", address3 = "Teststreet 3";
        unsigned int capacity3 = 300, inhabitants3 = 3000;
        VaccinationCenter* center3 = new VaccinationCenter(name3, address3, inhabitants3, capacity3);
        vaccinationCenters.push_back(center3);

        // Setup JSON Vaccines
        Vaccine vaccine1 = Vaccine("Vaccin 1", 0, 1, 2);
        Vaccine vaccine2 = Vaccine("Vaccin 2", 3, 4, 5);
        vaccines.push_back(new JValue(vaccine1.toJSON()));
        vaccines.push_back(new JValue(vaccine2.toJSON()));

    }

    // TODO: uncommenting this causes segmentation fault
//    virtual void TearDown() {
//        ITERATE(VaccinationCenters, vaccinationCenters, center) {
//            delete *center;
//        }
//        ITERATE(JValues, vaccines, vaccine) {
//            delete *vaccine;
//        }
//    }
};

/**
Tests the default constructor.
*/
TEST_F(HubTests, DefaultConstructor) {
//    EXPECT_EQ(1, 1);
    Hub defaultConstructor = Hub();
    EXPECT_TRUE(defaultConstructor.properlyInitialized());
    EXPECT_TRUE(defaultConstructor.getVaccines().empty());
    EXPECT_TRUE(defaultConstructor.getVaccinationCenters().empty());
    EXPECT_EQ((unsigned int) 0, defaultConstructor.getTotalVaccinesCount());
}

TEST_F(HubTests, LoadFromJSON) {
    Hub jsonHub = Hub();
    jsonHub.fromJSON(MockObjects::jHub(1, 2, 3, centerNames), vaccinationCenters);
    EXPECT_TRUE(jsonHub.properlyInitialized());
    EXPECT_EQ(centerNames.size(), jsonHub.getVaccinationCenters().size());
    EXPECT_EQ((unsigned int) 1, jsonHub.getVaccines().size());
    Vaccine* vaccine = jsonHub.getVaccines().front();
    EXPECT_EQ((unsigned int) 1, vaccine->getDelivery());
    EXPECT_EQ((unsigned int) 2, vaccine->getInterval());
    EXPECT_EQ((unsigned int) 3, vaccine->getTransportation());
}

TEST_F(HubTests, LoadFromJSONVaccineTypes) {
    Hub jsonHub = Hub();
    jsonHub.fromJSON(MockObjects::jHub(vaccines, centerNames), vaccinationCenters);
    EXPECT_TRUE(jsonHub.properlyInitialized());
    EXPECT_EQ(centerNames.size(), jsonHub.getVaccinationCenters().size());
    EXPECT_EQ((unsigned int) 2, jsonHub.getVaccines().size());
    std::vector<Vaccine*> vaccines = jsonHub.getVaccines();
    ITERATE(std::vector<Vaccine*>, vaccines, vaccine) {
        EXPECT_EQ((unsigned int) 0, (*vaccine)->getDelivery()%3);
        EXPECT_EQ((unsigned int) 1, (*vaccine)->getInterval()%3);
        EXPECT_EQ((unsigned int) 2, (*vaccine)->getTransportation()%3);
    }
}

TEST_F(HubTests, LoadFromJSONFail) {
    JObject* jObject = new JObject();
    Hub failHub = Hub();
    EXPECT_DEATH(failHub.fromJSON(jObject, vaccinationCenters), ".*should contain field.*");
    jObject->insertValue("centra", NULL);
    centerNames.push_back("RandomNameThatDoesntExist");
    EXPECT_DEATH(failHub.fromJSON(MockObjects::jHub(vaccines, centerNames), vaccinationCenters), ".*invalid name.*");
    centerNames.pop_back();
}


/**
 * Test the Happy Day scenario
 */
TEST_F(HubTests, HappyDay){
//    unsigned int initialVaccins = 20000, transport = 50;
//    unsigned int days = 10, centers = 3;
//    hub. = 300;
//    hub.interval = 7;
//    hub.transport = transport;
//    hub.vaccinsCount = initialVaccins;
//    unsigned int capacity = 100;
//    for(unsigned int i = 0; i < centers; i++) {
//        VaccinationCenter* center = new MockVaccinationCenter(capacity);
//        hub.centers.push_back(center);
//    }
//    for(unsigned int day = 0; day < days; day++)
//        hub.simulateDay(day);
//    unsigned int expectedLoss = (2*centers*capacity) + (centers*capacity*(days-1));
//    unsigned int expectedGain = hub.delivery*((days/hub.interval)+1);
//    EXPECT_TRUE(hub.vaccinsCount == initialVaccins - expectedLoss + expectedGain);
}
