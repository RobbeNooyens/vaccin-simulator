// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include "Vaccin.h"
#include "../DesignByContract.h"
#include "../json/JObject.h"

Vaccin::Vaccin(): initCheck(this) {
    ENSURE(properlyInitialized(), "Vaccin object wasn't initialized properly!");
}

bool Vaccin::properlyInitialized() const {
    return initCheck == this;
}

void Vaccin::fromJSON(JObject *json) {
    REQUIRE(properlyInitialized(), "Vaccin object wasn't initialized properly!");
    REQUIRE(json != NULL, "JObject can't be NULL!");
}

std::string Vaccin::getType() const {
    REQUIRE(properlyInitialized(), "Vaccin object wasn't initialized properly!");
    return type;
}

unsigned int Vaccin::getDelivery() const {
    REQUIRE(properlyInitialized(), "Vaccin object wasn't initialized properly!");
    return delivery;
}

unsigned int Vaccin::getInterval() const {
    REQUIRE(properlyInitialized(), "Vaccin object wasn't initialized properly!");
    return interval;
}

unsigned int Vaccin::getTransportation() const {
    REQUIRE(properlyInitialized(), "Vaccin object wasn't initialized properly!");
    return transportation;
}

unsigned int Vaccin::getRenewing() const {
    REQUIRE(properlyInitialized(), "Vaccin object wasn't initialized properly!");
    return renewing;
}

double Vaccin::getTemperature() const {
    REQUIRE(properlyInitialized(), "Vaccin object wasn't initialized properly!");
    return temperature;
}
