//
// Created by robbe on 18/04/2021.
//

#include "TestUtils.h"

#include "../json/JObject.h"
#include "../json/JValue.h"
#include "../json/JArray.h"
#include "../json/JKeys.h"

#define ITERATE(type, iteratable, name) for(type::iterator name = iteratable.begin(); name != iteratable.end(); name++)

JObject *MockObjects::jCenter(std::string name, std::string address, unsigned int inhabitants,
                                            unsigned int capacity) {
    JObject* center = new JObject();
    center->insertValue(CENTER_NAME, new JValue(name));
    center->insertValue(CENTER_ADDRESS, new JValue(address));
    center->insertValue(CENTER_INHABITANTS, new JValue(inhabitants));
    center->insertValue(CENTER_CAPACITY, new JValue(capacity));
    return center;
}

JObject *MockObjects::jHub(unsigned int delivery, unsigned int interval, unsigned int transportation, std::vector<std::string> centers) {
    JObject* hub = new JObject();
    hub->insertValue(HUB_DELIVERY, new JValue(delivery));
    hub->insertValue(HUB_INTERVAL, new JValue(interval));
    hub->insertValue(HUB_TRANSPORTATION, new JValue(transportation));

    JArray* centersArray = new JArray();
    ITERATE(std::vector<std::string>, centers, center)
        centersArray->insertValue(new JValue(*center));
    hub->insertValue(HUB_CENTERS, new JValue(centersArray));

    JArray* vaccins = new JArray();
    JObject* vaccine = new JObject();
    vaccine->insertValue(VACCINE_TYPE, new JValue("Standaard"));
    vaccine->insertValue(VACCINE_DELIVERY, new JValue(delivery));
    vaccine->insertValue(VACCINE_INTERVAL, new JValue(interval));
    vaccine->insertValue(VACCINE_TRANSPORTATION, new JValue(transportation));
    vaccins->insertValue(new JValue(vaccine));
    hub->insertValue(HUB_VACCINES, new JValue(vaccins));

    return hub;
}

JObject *MockObjects::jHub(JValues vaccines, std::vector<std::string> centers) {
    JObject* hub = new JObject();

    JArray* centersArray = new JArray();
    ITERATE(std::vector<std::string>, centers, center)
        centersArray->insertValue(new JValue(*center));
    hub->insertValue(HUB_CENTERS, new JValue(centersArray));

    JArray* vaccinesArray = new JArray();
    ITERATE(JValues, vaccines, vaccine)
        vaccinesArray->insertValue(new JValue((*vaccine)->asJObject()));
    hub->insertValue(HUB_VACCINES, new JValue(vaccinesArray));

    return hub;
}

JObject *
MockObjects::jVaccine(std::string type, unsigned int delivery, unsigned int interval, unsigned int transportation,
                      unsigned int renewing, double temperature) {
    JObject* vaccine = new JObject();
    vaccine->insertValue(VACCINE_TYPE, new JValue(type));
    vaccine->insertValue(VACCINE_DELIVERY, new JValue(delivery));
    vaccine->insertValue(VACCINE_INTERVAL, new JValue(interval));
    vaccine->insertValue(VACCINE_TRANSPORTATION, new JValue(transportation));
    vaccine->insertValue(VACCINE_RENEWING, new JValue(renewing));
    vaccine->insertValue(VACCINE_TEMPERATURE, new JValue(temperature));
    return vaccine;
}
