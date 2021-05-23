// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛
#include "JArray.h"
#include "JValue.h"
#include "../../DesignByContract.h"

void JArray::insertValue(JValue * val) {
    REQUIRE(properlyInitialized(), "JArray object hasn't been initialized properly!");
    items.push_back(val);
}

std::vector<JValue *> JArray::getItems() {
    REQUIRE(properlyInitialized(), "JArray object hasn't been initialized properly!");
    return items;
}

JArray::~JArray() {
    for(int i = 0; i < (int) items.size(); i++) {
        delete items[i];
        items[i] = NULL;
    }
}

JArray::JArray(): initCheck(this) {
    items = std::vector<JValue*>();
    ENSURE(properlyInitialized(), "JArray object hasn't been initialized properly!");
}

bool JArray::properlyInitialized() {
    return initCheck == this;
}
