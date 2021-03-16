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
public:
    explicit MockVaccinationCenter(int capacity): VaccinationCenter() {
        this->capacity = capacity;
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
    EXPECT_FALSE(hub.delivery || hub.interval || hub.transport || hub.vaccins);
    EXPECT_TRUE(hub.centers.empty());
}

/**
 * Test the Happy Day
 */
TEST_F(HubTests, HappyDay){
    unsigned int initialVaccins = 2000, transport = 60;
    unsigned int days = 5;
    hub.delivery = 300;
    hub.interval = 7;
    hub.transport = transport;
    hub.vaccins = initialVaccins;
    int capacity = 10;
    for(int i = 0; i < 5; i++) {
        VaccinationCenter* center = new MockVaccinationCenter(capacity);
        hub.centers.push_back(center);
    }
    for(unsigned int day = 0; day < days; day++)
        hub.simulateDay(day);
    EXPECT_TRUE(hub.vaccins == initialVaccins - (5*transport) + hub.delivery);
}
