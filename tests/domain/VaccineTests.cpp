// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛


#include <gtest/gtest.h>
#include "../../entities/Vaccine.h"
#include "../TestUtils.h"

#define TYPE "Pfizer"
#define DELIVERY 1
#define INTERVAL 2
#define TRANSPORTATION 3
#define RENEWING 4
#define TEMPERATURE 5.0

class VaccineTests: public ::testing::Test {};


TEST_F(VaccineTests, DefaultConstructors) {
    Vaccine defaultConstructor = Vaccine();
    EXPECT_TRUE(defaultConstructor.properlyInitialized());
    EXPECT_EQ(0, defaultConstructor.getType());
    EXPECT_EQ(0, defaultConstructor.getDelivery());
    EXPECT_EQ(0, defaultConstructor.getInterval());
    EXPECT_EQ(0, defaultConstructor.getTransportation());
    EXPECT_EQ(0, defaultConstructor.getRenewing());
    EXPECT_EQ(0.0, defaultConstructor.getTemperature());
}

TEST_F(VaccineTests, ParameterConstructors) {
    Vaccine parameterConstructor = Vaccine(TYPE, DELIVERY, INTERVAL, TRANSPORTATION, RENEWING, TEMPERATURE);
    EXPECT_TRUE(parameterConstructor.properlyInitialized());
    EXPECT_EQ(TYPE, parameterConstructor.getType());
    EXPECT_EQ(DELIVERY, parameterConstructor.getDelivery());
    EXPECT_EQ(INTERVAL, parameterConstructor.getInterval());
    EXPECT_EQ(TRANSPORTATION, parameterConstructor.getTransportation());
    EXPECT_EQ(RENEWING, parameterConstructor.getRenewing());
    EXPECT_EQ(TEMPERATURE, parameterConstructor.getTemperature());
}

TEST_F(VaccineTests, LoadFromJSON) {
    Vaccine jsonVaccine = Vaccine();
    jsonVaccine.fromJSON(MockObjects::jVaccine(TYPE, DELIVERY, INTERVAL, TRANSPORTATION, RENEWING, TEMPERATURE));
    EXPECT_TRUE(jsonVaccine.properlyInitialized());
    EXPECT_EQ(TYPE, jsonVaccine.getType());
    EXPECT_EQ(DELIVERY, jsonVaccine.getDelivery());
    EXPECT_EQ(INTERVAL, jsonVaccine.getInterval());
    EXPECT_EQ(TRANSPORTATION, jsonVaccine.getTransportation());
    EXPECT_EQ(RENEWING, jsonVaccine.getRenewing());
    EXPECT_EQ(TEMPERATURE, jsonVaccine.getTemperature());
}