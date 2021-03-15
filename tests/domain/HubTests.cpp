// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛
#include <gtest/gtest.h>
#include "../../entities/VaccinationCenter.h"
#include "../../entities/Hub.h"
//#include "../../utils.h"

//class MockVaccinationCenter: public VaccinationCenter {
//public:
//    MockVaccinationCenter(): VaccinationCenter() {
//        std::string counterAsString;
//        intutil::toString(counter, counterAsString);
//        stringutil::concat("Center", counterAsString, this->name);
//        stringutil::concat(this->name, "Street", this->address);
//        this->capacity = 100 + counter*10;
//        this->vaccins = 0;
//        this->inhabitants = 2000 + counter*50;
//        this->vaccinated = 0;
//        counter++;
//    }
//
//private:
//    static unsigned int counter;
//};

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
//    hub.delivery = 300;
//    hub.interval = 5;
//    hub.transport = 60;
//    hub.vaccins = 2000;
//    for(int i = 0; i < 5; i++) {
//        MockVaccinationCenter* center = new MockVaccinationCenter();
//        hub.centers.push_back(center);
//    }

}
