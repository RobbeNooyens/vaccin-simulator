// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#include "Vaccine.h"
#include "../../DesignByContract.h"
#include "../json/JObject.h"
#include "../json/JValue.h"
#include "../json/JKeys.h"

// Constructors

Vaccine::Vaccine() : initCheck(this), type(DEFAULT_VACCINE), delivery(0), interval(0), transportation(0), renewing(0), temperature(0) {
    ENSURE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
}

Vaccine::Vaccine(std::string type, unsigned int delivery, unsigned int interval, unsigned int transportation,
                 unsigned int renewing, double temperature) : initCheck(this), type(type), delivery(delivery), interval(interval),
                                                              transportation(transportation), renewing(renewing),
                                                              temperature(temperature) {
    ENSURE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
}

bool Vaccine::properlyInitialized() const {
    return initCheck == this;
}

// IO

void Vaccine::fromJSON(JObject *json) {
    REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
    REQUIRE(json, "JObject can't be NULL!");
    REQUIRE(json->contains(VACCINE_TYPE), "Vaccine JSON should contain field 'type'");
    REQUIRE(json->contains(VACCINE_DELIVERY), "Vaccine JSON should contain field 'levering'");
    REQUIRE(json->contains(VACCINE_INTERVAL), "Vaccine JSON should contain field 'interval'");
    REQUIRE(json->contains(VACCINE_TRANSPORTATION), "Vaccine JSON should contain field 'transport'");
    type = json->getValue(VACCINE_TYPE)->asString();
    delivery = json->getValue(VACCINE_DELIVERY)->asUnsignedint();
    interval = json->getValue(VACCINE_INTERVAL)->asUnsignedint();
    transportation = json->getValue(VACCINE_TRANSPORTATION)->asUnsignedint();
    renewing = json->contains(VACCINE_RENEWING) ? json->getValue(VACCINE_RENEWING)->asUnsignedint() : 0;
    temperature = json->contains(VACCINE_TEMPERATURE) ? json->getValue(VACCINE_TEMPERATURE)->asDouble() : 0;
    ENSURE(getType() == json->getValue(VACCINE_TYPE)->asString(), "Vaccine type didn't load properly!");
    ENSURE(getDelivery() == json->getValue(VACCINE_DELIVERY)->asUnsignedint(), "Vaccine delivery didn't load properly!");
    ENSURE(getInterval() == json->getValue(VACCINE_INTERVAL)->asUnsignedint(), "Vaccine interval didn't load properly!");
    ENSURE(getTransportation() == json->getValue(VACCINE_TRANSPORTATION)->asUnsignedint(), "Vaccine transportation didn't load properly!");
    ENSURE(getRenewing() == json->getValue(VACCINE_RENEWING)->asUnsignedint(), "Vaccine renewing didn't load properly!");
    ENSURE(getTemperature() == json->getValue(VACCINE_TEMPERATURE)->asDouble(), "Vaccine temperature didn't load properly!");
}

// Getters

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

// Smart distribution

bool Vaccine::operator<(const Vaccine& s) const {
    REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
    return transportation < s.getTransportation();
}

unsigned int Vaccine::getDynamicDelivery() const {
    REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
    return dynamicDelivery;
}

Hub* Vaccine::getHub() const {
    REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
    return hub;
}

void Vaccine::setHub(Hub* h) {
    REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
    REQUIRE(h, "Hub connected to the vaccine cannot be NULL!");
    hub = h;
}

std::vector<unsigned int>& Vaccine::getDays() {
    REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
    return days;
}
