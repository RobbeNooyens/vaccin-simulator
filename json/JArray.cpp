//
// Created by robbe on 16/03/2021.
//

#include "JArray.h"

JArray::JArray() {
}

void JArray::append(JValue * val) {
    items.push_back(val);
}

std::vector<JValue *> &JArray::getItems() {
    return items;
}

JArray::~JArray() {

}
