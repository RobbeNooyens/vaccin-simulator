//
// Created by robbe on 16/03/2021.
//

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