//
// Created by robbe on 18/04/2021.
//

#include "TestUtils.h"

#include "../json/JObject.h"
#include "../json/JValue.h"

JObject *MockObjects::jCenter(std::string name, std::string address, unsigned int inhabitants,
                                            unsigned int capacity) {
    JObject* centerObject = new JObject();
    centerObject->insertValue("naam", new JValue(name));
    centerObject->insertValue("adres", new JValue(address));
    centerObject->insertValue("inwoners", new JValue(inhabitants));
    centerObject->insertValue("capaciteit", new JValue(capacity));
    return centerObject;
}

JObject *MockObjects::jHub(unsigned int delivery, unsigned int interval, unsigned int transportation) {
    JObject* h = new JObject();
    h->insertValue("levering", new JValue(delivery));
    h->insertValue("interval", new JValue(interval));
    h->insertValue("transport", new JValue(transportation));
    return h;
}
