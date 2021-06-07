// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#include <gtest/gtest.h>
#include "../../src/entities/VaccinationCenter.h"
#include "../../src/json/JObject.h"
#include "../../src/json/JValue.h"
#include "../TestUtils.h"
#include "../../src/utilities/utils.h"
#include "../../src/entities/SimulationData.h"

#define INHABITANTS (unsigned int) 6000
#define CAPACITY (unsigned int) 200
#define NAME "TestCenter"
#define ADDRESS "TestStreet"
#define TRANSPORT (unsigned int) 100

class VaccinationCenterTests: public ::testing::Test {
protected:
    Vaccine* vaccine;
    SimulationData statistics;

    void SetUp() {
        vaccine = new Vaccine("Test", 1,0,1);
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
}

/**
Tests the happy day scenario.
*/
TEST_F(VaccinationCenterTests, HappyDay){
    VaccinationCenter c = VaccinationCenter(NAME, ADDRESS, INHABITANTS, CAPACITY);
    unsigned int backupVaccins = c.getVaccins();
    c.transportationArrived(vaccine, TRANSPORT);
    EXPECT_EQ(backupVaccins + TRANSPORT, c.getVaccins());
    EXPECT_FALSE(vaccine->getType().empty());
    c.vaccinateInhabitants(0, statistics);
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
        c.vaccinateInhabitants(i, statistics);
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
    delete centerObject;
}

TEST_F(VaccinationCenterTests, BadConstructor) {
    EXPECT_DEATH(VaccinationCenter("","correctAddress",500,500),"Name cannot be empty!");
    EXPECT_DEATH(VaccinationCenter("correctName","",500,500),"Address cannot be empty!");
    EXPECT_DEATH(VaccinationCenter("correctName","correctAddress",0,500),"Center can't have zero inhabitants!");
    EXPECT_DEATH(VaccinationCenter("correctName","correctAddress",500,0),"Center can't have a capacity of 0!");
}

TEST_F(VaccinationCenterTests, RemoveExpiredVaccines) {
    VaccinationCenter * centre = new VaccinationCenter("Center 1","Address 1",100,100);
    Vaccine * vaccine = new Vaccine("Vaccine 1",100,100,100,100,-1);
    centre->transportationArrived(vaccine, 1000);
    centre->removeExpiredVaccines();
    EXPECT_EQ((unsigned int) 0, centre->getVaccinesMap()[vaccine]);
    delete centre;
    delete vaccine;
}

TEST_F(VaccinationCenterTests, Getters) {
    std::string name = "Center 1";
    std::string address = "Address 1";
    unsigned int inhabitants = 100;
    unsigned int capacity = 100;
    VaccinationCenter * centre = new VaccinationCenter(name,address,inhabitants,capacity);
    EXPECT_EQ(name, centre->getName());
    EXPECT_EQ(address, centre->getAddress());
    EXPECT_EQ(inhabitants, centre->getInhabitants());
    EXPECT_EQ(capacity, centre->getCapacity());
    EXPECT_EQ((unsigned int) 0, centre->getVaccins());
    EXPECT_EQ(0.0, centre->getPercentageVaccines());
    EXPECT_EQ(0.0, centre->getPercentageVaccinated());
    EXPECT_EQ(centre->getInhabitants(), centre->getVaccinationsLeft());
    delete centre;
}
