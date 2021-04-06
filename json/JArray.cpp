// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛
#include "JArray.h"
#include "JValue.h"
#include "../DesignByContract.h"

void JArray::insertValue(JValue * val) {
    REQUIRE(properlyInitialized(), "JArray object hasn't been initialized properly!");
    items.push_back(val);
}

std::vector<JValue *> &JArray::getItems() {
    REQUIRE(properlyInitialized(), "JArray object hasn't been initialized properly!");
    return items;
}

JArray::~JArray() {
    REQUIRE(properlyInitialized(), "JArray object hasn't been initialized properly!");
    for(int i = 0; i < (int) items.size(); i++) {
        delete items[i];
        items[i] = NULL;
    }
}

JArray::JArray(): initCheck(this) {
    ENSURE(properlyInitialized(), "JArray object hasn't been initialized properly!");
}

bool JArray::properlyInitialized() {
    return initCheck == this;
}
