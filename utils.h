// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#ifndef VACCIMULATOR_UTILS_H
#define VACCIMULATOR_UTILS_H

#include <string>

namespace stringutil {

    /**
     * Returns true if the target string is a substring of the source string
     * @param source std::string; String that can contain the target string
     * @param target std::string; String that can be a substring of the source string
     * @return bool; true if the source string contains the target string
     */
    bool contains(const std::string &source, const std::string &target) {
        return source.find(target) != std::string::npos;
    }

}
#endif //VACCIMULATOR_UTILS_H
