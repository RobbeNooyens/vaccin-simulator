//
// Created by robbe on 10/03/2021.
//

#include <gtest/gtest.h>
#include "../../entities/VaccinationCenter.h"
#include "../../entities/Hub.h"

class VaccinationCenterTests: public ::testing::Test {
protected:
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.
    VaccinationCenter center;

};

/**
Tests the default constructor.
*/
TEST_F(VaccinationCenterTests, DefaultConstructor) {
    EXPECT_TRUE(center.properlyInitialized());
    // Check initial values
    // These fields should be 0 after initialization.
    EXPECT_FALSE(center.getVaccins() || center.getInhabitants() || center.getVaccinationsDone() || center.getCapacity());
    EXPECT_TRUE(center.getName().empty() && center.getAddress().empty());
}

/**
Tests constructors with parameter fields.
*/
TEST_F(VaccinationCenterTests, ParameterConstructors) {
    std::string name = "TestCenter", address = "TestStreet";
    unsigned int inhabitants = 1000, capacity = 200;
    VaccinationCenter c = VaccinationCenter(name, address, inhabitants, capacity);
    EXPECT_TRUE(c.properlyInitialized());
    EXPECT_TRUE(c.getName() == name && c.getAddress() == address);
    EXPECT_TRUE(c.getInhabitants() == inhabitants && c.getCapacity() == capacity);
    EXPECT_TRUE(c.getVaccins() == 0);
}

/**
Tests the happy day scenario.
*/
TEST_F(VaccinationCenterTests, HappyDay){
    const unsigned int transport = 150, inhabitants = 3000, capacity = 100, backupVaccins = 0;
    VaccinationCenter c = VaccinationCenter("TestCenter", "TestStreet", inhabitants, capacity);
    c.transportationArrived(transport);
    EXPECT_TRUE(c.getVaccins() == backupVaccins + transport);
    c.vaccinateInhabitants();
    EXPECT_TRUE(c.getVaccinationsDone() == std::min(transport, capacity));
    EXPECT_TRUE(c.getVaccinationsLeft() == (inhabitants - std::min(transport, capacity)));
}

/**
 * Tests transportation
 */
TEST_F(VaccinationCenterTests, Transportation){
    unsigned int transport = 10, inhabitants = 3000, capacity = 20, vaccins = 0;
    VaccinationCenter c = VaccinationCenter("TestCenter", "TestStreet", inhabitants, capacity);
    for(int i = 0; i < 10; i++){
        vaccins += transport;
        c.transportationArrived(transport);
        EXPECT_TRUE(c.getVaccins() == vaccins);
    }
}

/**
 * Tests vaccinations
 */
TEST_F(VaccinationCenterTests, vaccinations){
    unsigned int inhabitants = 3000, capacity = 200, vaccins = 3000;
    VaccinationCenter c = VaccinationCenter("TestCenter", "TestStreet", inhabitants, capacity);
    c.transportationArrived(vaccins);
    for(int i = 1; i <= 15; i++) {
        c.vaccinateInhabitants();
        vaccins -= capacity;
        EXPECT_TRUE(c.getVaccins() == vaccins);
        EXPECT_TRUE(c.getVaccinationsDone() == i*capacity);
    }
}