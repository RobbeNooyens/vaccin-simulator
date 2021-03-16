// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛
#include "JArray.h"

void JArray::insertValue(JValue * val) {
    items.push_back(val);
}

std::vector<JValue *> &JArray::getItems() {
    return items;
}

JArray::~JArray() {

}

JArray::JArray() {
}