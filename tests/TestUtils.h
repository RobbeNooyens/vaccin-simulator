//
// Created by robbe on 18/04/2021.
//

#ifndef VACCIN_SIMULATOR_TESTUTILS_H
#define VACCIN_SIMULATOR_TESTUTILS_H

#include <string>

class JObject;

namespace MockObjects {
    JObject* jHub(unsigned int delivery, unsigned int interval, unsigned int transportation);

    JObject* jCenter(std::string name, std::string address, unsigned int inhabitants, unsigned int capacity);

};


#endif //VACCIN_SIMULATOR_TESTUTILS_H
