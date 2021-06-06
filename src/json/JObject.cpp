// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#include "JObject.h"
#include "JValue.h"
#include "../../DesignByContract.h"
#include "../utilities/utils.h"
#include <string>

JValue* JObject::getValue(const std::string& key) {
    REQUIRE(properlyInitialized(), "JObject object hasn't been initialized properly!");
    REQUIRE(!key.empty(), "JObject key can't be empty!");
    int index = key.find_first_of('.');
    if(index != (int) std::string::npos && index < (int) key.size()-1) {
        std::string first = key.substr(0, index);
        std::string second = key.substr(index+1, key.size()-1);
        JObject* parent = values[first]->asJObject();
        return parent->getValue(second);
    }
    if(!(MAP_CONTAINS_KEY(values, key)))
        return NULL;
    return values[key];
}

JObject::JObject(): initCheck(this) {
    values = std::map<std::string, JValue*>();
    ENSURE(properlyInitialized(), "JObject object hasn't been initialized properly!");
}

void JObject::insertValue(std::string key, JValue * value) {
    REQUIRE(properlyInitialized(), "JObject object hasn't been initialized properly!");
    REQUIRE(!key.empty(), "JObject key can't be empty!");
    REQUIRE(value != NULL, "JObject value can't be NULL!");
    values.insert(std::pair<std::string, JValue*>(key, value));
}

bool JObject::contains(std::string key) {
    REQUIRE(properlyInitialized(), "JObject object hasn't been initialized properly!");
    REQUIRE(!key.empty(), "JObject key can't be empty!");
    return getValue(key) != NULL;
}

bool JObject::containsAll(std::vector<std::string> keys) {
    REQUIRE(properlyInitialized(), "JObject object hasn't been initialized properly!");
    for(int i = 0; i < (int) keys.size(); i++){
        if(keys[i].empty())
            continue;
        if(!contains(keys[i]))
            return false;
    }
    return true;
}

bool JObject::properlyInitialized() {
    return initCheck == this;
}

JObject::~JObject() {
    REQUIRE(properlyInitialized(), "JObject object hasn't been initialized properly!");
    if(!values.empty()){
        for(std::map<std::string, JValue*>::iterator it = values.begin(); it != values.end(); it++) {
            delete it->second;
            it->second = NULL;
        }
    }
    values.clear();
    ENSURE(values.empty(), "JObject values weren't deleted properly!");
}
