// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include <gtest/gtest.h>
#include "../../entities/Vaccine.h"
#include "../../entities/Hub.h"
#include "../../json/JObject.h"
#include "../../json/JValue.h"
#include "../TestUtils.h"

#define INHABITANTS (unsigned int) 3000
#define CAPACITY (unsigned int) 200
#define NAME "TestCenter"
#define ADDRESS "TestStreet"
#define TRANSPORT (unsigned int) 150

class VaccineTests: public ::testing::Test {
protected:
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.
//    Vaccine vaccine;

};

/**
Tests the default constructor.
*/
TEST_F(VaccineTests, DefaultConstructor) {
    Vaccine vaccine = Vaccine();
    EXPECT_TRUE(vaccine.properlyInitialized());
    // Check initial values
    // These fields should be 0 after initialization.
    EXPECT_FALSE(vaccine.getDelivery() || vaccine.getInterval() || vaccine.getTransportation() || vaccine.getRenewing() || vaccine.getTemperature() || vaccine.getDynamicDelivery() || vaccine.getType());
    EXPECT_TRUE(vaccine.getType().empty() && vaccine.getDays().empty());
}

/**
Tests the happy day scenario.
*/
TEST_F(VaccineTests, HappyDay){
    Vaccine vaccine = Vaccine();
    Hub* hub = new Hub;
    unsigned int backupVaccins = c.getVaccins();
    c.transportationArrived(TRANSPORT);
    EXPECT_EQ(backupVaccins + TRANSPORT, c.getVaccins());
    c.vaccinateInhabitants();
    EXPECT_EQ(std::min(TRANSPORT, CAPACITY), c.getVaccinationsDone());
    EXPECT_EQ(INHABITANTS - std::min(TRANSPORT, CAPACITY), c.getVaccinationsLeft());
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
    EXPECT_DEATH(c.fromJSON(centerObject), ".*VaccinationCenter JSON should contain field 'naam'.*");
    centerObject->insertValue("naam", new JValue("Test"));
    EXPECT_DEATH(c.fromJSON(centerObject), ".*VaccinationCenter JSON should contain field 'adres'.*");
    centerObject->insertValue("adres", new JValue("Street"));
    EXPECT_DEATH(c.fromJSON(centerObject), ".*VaccinationCenter JSON should contain field 'inwoners'.*");
    centerObject->insertValue("inwoners", new JValue((unsigned int) 1000));
    EXPECT_DEATH(c.fromJSON(centerObject), ".*VaccinationCenter JSON should contain field 'capaciteit'.*");
    centerObject->insertValue("capaciteit", new JValue((unsigned int) 100));
    EXPECT_NO_FATAL_FAILURE(c.fromJSON(centerObject));
    // TODO: delete centerObject without segmentation fault
//    delete centerObject;
}
