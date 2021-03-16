// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include "JObject.h"
#include <string>

JValue* JObject::getValue(const std::string& key) {
    return values[key];
}

JObject::JObject() {

}

void JObject::insertValue(std::string key, JValue * value) {
    values.insert(std::pair<std::string, JValue*>(key, value));
}
