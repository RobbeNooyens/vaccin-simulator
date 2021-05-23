// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#include <gtest/gtest.h>
#include "../../entities/VaccinationCenter.h"
#include "../../entities/Hub.h"
#include "../../json/JObject.h"
#include "../../json/JValue.h"
#include "../TestUtils.h"
#include "../../src/utilities/utils.h"

#define INHABITANTS (unsigned int) 6000
#define CAPACITY (unsigned int) 200
#define NAME "TestCenter"
#define ADDRESS "TestStreet"
#define TRANSPORT (unsigned int) 100

class VaccinationCenterTests: public ::testing::Test {
protected:
    Vaccine* vaccine;

    void SetUp() {
        vaccine = new Vaccine();
        CLOSE_COUT;
    }

    void TearDown() {
        delete vaccine;
        OPEN_COUT;
    }
};

/**
Tests the default constructor.
*/
TEST_F(VaccinationCenterTests, DefaultConstructor) {
    VaccinationCenter center = VaccinationCenter();
    EXPECT_TRUE(center.properlyInitialized());
    // Check initial values
    // These fields should be 0 after initialization.
    EXPECT_EQ((unsigned int) 0, center.getVaccins());
    EXPECT_EQ((unsigned int) 0, center.getInhabitants());
    EXPECT_EQ((unsigned int) 0, center.getVaccinationsDone());
    EXPECT_EQ((unsigned int) 0, center.getCapacity());
    EXPECT_EQ(&std::cout, center.getOutputstream());
    EXPECT_TRUE(center.getName().empty());
    EXPECT_TRUE(center.getAddress().empty());
}

/**
Tests constructors with parameter fields.
*/
TEST_F(VaccinationCenterTests, ParameterConstructors) {
    VaccinationCenter c = VaccinationCenter(NAME, ADDRESS, INHABITANTS, CAPACITY);
    EXPECT_TRUE(c.properlyInitialized());
    EXPECT_EQ(NAME, c.getName());
    EXPECT_EQ(ADDRESS, c.getAddress());
    EXPECT_EQ(INHABITANTS, c.getInhabitants());
    EXPECT_EQ(CAPACITY, c.getCapacity());
    EXPECT_EQ((unsigned int) 0, c.getVaccins());
    EXPECT_EQ(&std::cout, c.getOutputstream());
}

/**
Tests the happy day scenario.
*/
TEST_F(VaccinationCenterTests, HappyDay){
    VaccinationCenter c = VaccinationCenter(NAME, ADDRESS, INHABITANTS, CAPACITY);
    unsigned int backupVaccins = c.getVaccins();
    c.transportationArrived(vaccine, TRANSPORT);
    EXPECT_EQ(backupVaccins + TRANSPORT, c.getVaccins());
    c.vaccinateInhabitants(0, NULLNULL);
    EXPECT_EQ(std::min(TRANSPORT, CAPACITY), c.getVaccinationsDone());
    EXPECT_EQ(INHABITANTS - std::min(TRANSPORT, CAPACITY), c.getVaccinationsLeft());
}

/**
 * Tests transportation
 */
TEST_F(VaccinationCenterTests, Transportation){
    unsigned int vaccins = 0;
    VaccinationCenter c = VaccinationCenter(NAME, ADDRESS, INHABITANTS, CAPACITY);
    unsigned int transport = 10;
    for(int i = 0; i < 10; i++){
        vaccins += transport;
        c.transportationArrived(vaccine, transport);
        EXPECT_EQ(vaccins, c.getVaccins());
    }
}

/**
 * Tests vaccinations
 */
TEST_F(VaccinationCenterTests, Vaccinations){
    VaccinationCenter c = VaccinationCenter(NAME, ADDRESS, INHABITANTS, CAPACITY);
    for(int i = 1; i <= 15; i++) {
        c.transportationArrived(vaccine, CAPACITY);
        c.vaccinateInhabitants(i, NULLNULL);
        EXPECT_EQ(i*CAPACITY, c.getVaccinationsDone());
    }
}

TEST_F(VaccinationCenterTests, FromJSON) {
    JObject* centerObject = MockObjects::jCenter(NAME, ADDRESS, INHABITANTS, CAPACITY);
    VaccinationCenter c = VaccinationCenter();
    c.fromJSON(centerObject);
    EXPECT_EQ(ADDRESS, c.getAddress());
    EXPECT_EQ(NAME,c.getName());
    EXPECT_EQ(CAPACITY, c.getCapacity());
    EXPECT_EQ(INHABITANTS, c.getInhabitants());
    delete centerObject;

}

TEST_F(VaccinationCenterTests, FailFromJSON) {
    VaccinationCenter c = VaccinationCenter();
    EXPECT_DEATH(c.fromJSON(NULL), ".*Json can't be NULL.*");
    JObject* centerObject = new JObject();
    EXPECT_DEATH(c.fromJSON(centerObject), ".*VaccinationCenter JSON should contain field naam.*");
    centerObject->insertValue("naam", new JValue("Test"));
    EXPECT_DEATH(c.fromJSON(centerObject), ".*VaccinationCenter JSON should contain field adres.*");
    centerObject->insertValue("adres", new JValue("Street"));
    EXPECT_DEATH(c.fromJSON(centerObject), ".*VaccinationCenter JSON should contain field inwoners.*");
    centerObject->insertValue("inwoners", new JValue((unsigned int) 1000));
    EXPECT_DEATH(c.fromJSON(centerObject), ".*VaccinationCenter JSON should contain field capaciteit.*");
    centerObject->insertValue("capaciteit", new JValue((unsigned int) 100));
    EXPECT_NO_FATAL_FAILURE(c.fromJSON(centerObject));
    // TODO: delete centerObject without segmentation fault
//    delete centerObject;
}