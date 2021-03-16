//
// Created by robbe on 16/03/2021.
//

#include "JObject.h"
#include <string>

JObject::JObject() {
}

JValue* JObject::getValue(const std::string& key) {
    return values.find(key)->second;
}

void JObject::put(std::string, JValue *) {
    values.insert(std::pair<std::string, JValue*>(key, value));
}
