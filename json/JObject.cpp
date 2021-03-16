//
// Created by robbe on 16/03/2021.
//

#include "JObject.h"
#include <string>

JObject::JObject(std::string key, JValue* value) {
    values.insert(std::pair<std::string, JValue*>(key, value));
}

JValue* JObject::getValue(const std::string& key) {
    return values.find(key)->second;
}