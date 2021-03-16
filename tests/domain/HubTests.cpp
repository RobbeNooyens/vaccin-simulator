// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛
#include <gtest/gtest.h>
#include "../../entities/VaccinationCenter.h"
#include "../../entities/Hub.h"

class MockVaccinationCenter: public VaccinationCenter {

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
    EXPECT_FALSE(hub.delivery || hub.interval || hub.transport || hub.vaccins);
    EXPECT_TRUE(hub.centers.empty());
}

TEST_F(HubTests, HappyDay){
    hub.delivery = 300;
    hub.interval = 5;
    hub.transport = 60;
    hub.vaccins = 2000;
    for(int i = 0; i < 5; i++) {
        MockVaccinationCenter* center = new MockVaccinationCenter();
        hub.centers.push_back(center);
    }

}
