// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#ifndef VACCIMULATOR_UTILS_H
#define VACCIMULATOR_UTILS_H

#include <string>
#include<iostream>
#include <sstream>  // for string streams

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

    /**
     * Concatenates s1 and s2 in result
     * @param s1 the left side of the concatenated string
     * @param s2 the right side of the concatenated string
     * @param result s1 and s2 concatenated
     */
    void concat(const std::string &s1, const std::string &s2, std::string &result) {
        std::stringstream ss;
        ss << s1 << s2;
        result = ss.str();
    }

}

namespace intutil {

    /**
     * Casts an int to a string
     * @param num is the number to be converted
     * @param target is the string that will contain the converted number as a string
     */
    void toString(int num, std::string &target) {
        std::ostringstream convert;   // stream used for the conversion
        convert << num;      // insert the textual representation of 'Number' in the characters in the stream
        target = convert.str();
    }
}
#endif //VACCIMULATOR_UTILS_H
