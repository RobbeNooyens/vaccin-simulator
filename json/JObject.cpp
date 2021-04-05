// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include "JObject.h"
#include "JValue.h"
#include <string>

JValue* JObject::getValue(const std::string& key) {
    int index = key.find_first_of('.');
    if(index != (int) std::string::npos && index < (int) key.size()-1) {
        std::string first = key.substr(0, index);
        std::string second = key.substr(index+1, key.size()-1);
        JObject* parent = values[first]->asJObject();
        return parent->getValue(second);
    }
    if(values.find(key) == values.end())
        return NULL;
    JValue* val = values[key];
    return val;
}

JObject::JObject() {

}

void JObject::insertValue(std::string key, JValue * value) {
    values.insert(std::pair<std::string, JValue*>(key, value));
}

bool JObject::contains(std::string key) {
    return getValue(key) != NULL;
}

bool JObject::containsAll(std::vector<std::string> keys) {
    for(int i = 0; i < (int) keys.size(); i++){
        if(!contains(keys[i])){
            return false;
        }
    }
    return true;
}

bool JObject::properlyInitialized() {
    return initCheck == this;
}

JObject::~JObject() {
    for(std::map<std::string, JValue*>::iterator it = values.begin(); it != values.end(); it++) {
        delete it->second;
    }
}
