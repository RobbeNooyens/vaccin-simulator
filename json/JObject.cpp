//
// Created by robbe on 16/03/2021.
//

#include "JObject.h"
#include <string>

JValue* JObject::getValue(const std::string& key) {
    return values.find(key)->second;
}

JObject::JObject() {

}

void JObject::insertValue(std::string key, JValue * value) {
    values.insert(std::pair<std::string, JValue*>(key, value));
}
