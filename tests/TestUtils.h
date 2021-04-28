//
// Created by robbe on 18/04/2021.
//

#ifndef VACCIN_SIMULATOR_TESTUTILS_H
#define VACCIN_SIMULATOR_TESTUTILS_H

#include <string>
#include <vector>

class JValue;
class JObject;

namespace MockObjects {
    JObject* jHub(unsigned int delivery, unsigned int interval, unsigned int transportation, std::vector<std::string> centers);
    JObject* jHub(std::vector<JValue*> vaccines, std::vector<std::string> centers);
    JObject* jCenter(std::string name, std::string address, unsigned int inhabitants, unsigned int capacity);
    JObject* jVaccine(std::string type, unsigned int delivery, unsigned int interval, unsigned int transportation, unsigned int renewing, double temperature);
}


#endif //VACCIN_SIMULATOR_TESTUTILS_H
