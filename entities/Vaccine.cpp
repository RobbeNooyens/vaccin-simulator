// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include "Vaccine.h"
#include "../DesignByContract.h"
#include "../json/JObject.h"
#include "../json/JValue.h"

Vaccine::Vaccine(): initCheck(this), dynamicDelivery(0) {
    ENSURE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
}

bool Vaccine::properlyInitialized() const {
    return initCheck == this;
}

void Vaccine::fromJSON(JObject *json) {
    REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
    REQUIRE(json != NULL, "JObject can't be NULL!");
    // TODO: documentation
    type = json->getValue("type")->asString();
    delivery = json->getValue("levering")->asUnsignedint();
    interval = json->getValue("interval")->asUnsignedint();
    transportation = json->getValue("transport")->asUnsignedint();
    renewing = json->getValue("hernieuwing")->asUnsignedint();
    temperature = json->getValue("temperatuur")->asDouble();

}

std::string Vaccine::getType() const {
    REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
    return type;
}

unsigned int Vaccine::getDelivery() const {
    REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
    return delivery;
}

unsigned int Vaccine::getInterval() const {
    REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
    return interval;
}

unsigned int Vaccine::getTransportation() const {
    REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
    return transportation;
}

unsigned int Vaccine::getRenewing() const {
    REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
    return renewing;
}

double Vaccine::getTemperature() const {
    REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
    return temperature;
}

bool Vaccine::operator<(const Vaccine& s) const {
    REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
    return transportation < s.getTransportation();
}

unsigned int getDynamicDelivery() const {
    REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
    return dynamicDelivery;

Hub* Vaccine::getHub() const {
    REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
    return hub;
}

void Vaccine::setHub(Hub* h) {
    REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
    hub = h;
}

vector<unsigned int>& Vaccine::getDays() const {
    REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
    return days;
}
