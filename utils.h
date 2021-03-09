//
// Created by robbe on 9/03/2021.
//

#ifndef VACCIMULATOR_UTILS_H
#define VACCIMULATOR_UTILS_H

#include <string>

bool stringContains(const std::string& source, const std::string& target) {
    return source.find(target) != std::string::npos;
}

#endif //VACCIMULATOR_UTILS_H
