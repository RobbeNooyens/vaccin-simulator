// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛
#include <gtest/gtest.h>
#include "../../entities/VaccinationCenter.h"
#include "../../entities/Hub.h"

class MockVaccinationCenter: public VaccinationCenter {
public:
    explicit MockVaccinationCenter(unsigned int capacity): VaccinationCenter("", "", 0, capacity) {
    }

};

class HubTests: public ::testing::Test {
protected:
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.
    Hub hub;
};

/**
Tests the default constructor.
*/
TEST_F(HubTests, DefaultConstructor) {
    EXPECT_TRUE(hub.properlyInitialized());
    // Check initial values
    // These fields should be 0 after initialization.
//    EXPECT_FALSE(hub || hub.interval || hub.transport || hub.vaccinsCount);
//    EXPECT_TRUE(hub.centers.empty());
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
