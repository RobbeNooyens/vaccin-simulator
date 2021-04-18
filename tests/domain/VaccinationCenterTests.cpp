// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include <gtest/gtest.h>
#include "../../entities/VaccinationCenter.h"
#include "../../entities/Hub.h"
#include "../../json/JObject.h"
#include "../../json/JValue.h"
#include "../TestUtils.h"

#define INHABITANTS (unsigned int) 3000
#define CAPACITY (unsigned int) 200
#define NAME "TestCenter"
#define ADDRESS "TestStreet"
#define TRANSPORT (unsigned int) 150

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
    center = VaccinationCenter();
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
    VaccinationCenter c = VaccinationCenter(NAME, ADDRESS, INHABITANTS, CAPACITY);
    EXPECT_TRUE(c.properlyInitialized());
    EXPECT_TRUE(c.getName() == NAME && c.getAddress() == ADDRESS);
    EXPECT_TRUE(c.getInhabitants() == INHABITANTS && c.getCapacity() == CAPACITY);
    EXPECT_TRUE(c.getVaccins() == 0);
}

/**
Tests the happy day scenario.
*/
TEST_F(VaccinationCenterTests, HappyDay){
    VaccinationCenter c = VaccinationCenter(NAME, ADDRESS, INHABITANTS, CAPACITY);
    unsigned int backupVaccins = c.getVaccins();
    c.transportationArrived(TRANSPORT);
    EXPECT_TRUE(c.getVaccins() == backupVaccins + TRANSPORT);
    c.vaccinateInhabitants();
    EXPECT_TRUE(c.getVaccinationsDone() == std::min(TRANSPORT, CAPACITY));
    EXPECT_TRUE(c.getVaccinationsLeft() == (INHABITANTS - std::min(TRANSPORT, CAPACITY)));
}

/**
 * Tests transportation
 */
TEST_F(VaccinationCenterTests, Transportation){
    unsigned int vaccins = 0;
    VaccinationCenter c = VaccinationCenter(NAME, ADDRESS, INHABITANTS, CAPACITY);
    for(int i = 0; i < 10; i++){
        vaccins += TRANSPORT;
        c.transportationArrived(TRANSPORT);
        EXPECT_TRUE(c.getVaccins() == vaccins);
    }
}

/**
 * Tests vaccinations
 */
TEST_F(VaccinationCenterTests, Vaccinations){
    unsigned int vaccins = 3000;
    VaccinationCenter c = VaccinationCenter(NAME, ADDRESS, INHABITANTS, CAPACITY);
    c.transportationArrived(vaccins);
    for(int i = 1; i <= 15; i++) {
        c.vaccinateInhabitants();
        vaccins -= CAPACITY;
        EXPECT_TRUE(c.getVaccins() == vaccins);
        EXPECT_TRUE(c.getVaccinationsDone() == i*CAPACITY);
    }
}

TEST_F(VaccinationCenterTests, FromJSON) {
    JObject* centerObject = MockObjects::jCenter(NAME, ADDRESS, INHABITANTS, CAPACITY);
    VaccinationCenter c = VaccinationCenter();
    c.fromJSON(centerObject);
    EXPECT_TRUE(c.getAddress() == ADDRESS);
    EXPECT_TRUE(c.getName() == NAME);
    EXPECT_TRUE(c.getCapacity() == CAPACITY);
    EXPECT_TRUE(c.getInhabitants() == INHABITANTS);
    delete centerObject;

}

TEST_F(VaccinationCenterTests, FailFromJSON) {
    VaccinationCenter c = VaccinationCenter();
    EXPECT_DEATH(c.fromJSON(NULL), "Center cannot be loaded from nullpointer!");
    JObject* centerObject = NULL;
    EXPECT_DEATH(c.fromJSON(centerObject), "Missing address!");
    centerObject->insertValue("adres", NULL);
    EXPECT_DEATH(c.fromJSON(centerObject), "Missing name!");
    centerObject->insertValue("name", NULL);
    EXPECT_DEATH(c.fromJSON(centerObject), "Missing inhabitants!");
    centerObject->insertValue("inhabitants", NULL);
    EXPECT_DEATH(c.fromJSON(centerObject), "Missing capacity!");
    centerObject->insertValue("capacity", NULL);
    EXPECT_NO_FATAL_FAILURE(c.fromJSON(centerObject));
}