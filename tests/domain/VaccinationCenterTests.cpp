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
    Hub hub;
    VaccinationCenter center;

};

/**
Tests the default constructor.
*/
TEST_F(VaccinationCenterTests, DefaultConstructor) {
    EXPECT_TRUE(center.properlyInitialized());
}