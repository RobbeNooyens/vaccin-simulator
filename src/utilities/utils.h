// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#ifndef VACCIMULATOR_UTILS_H
#define VACCIMULATOR_UTILS_H

#include <fstream>
#include <sys/stat.h>
#include <string>
#include <iostream>
#include <sstream>

#define MAP_CONTAINS_KEY(vector, element) vector.find(element) != vector.end()
#define CLOSE_COUT std::cout.setstate(std::ios_base::badbit)
#define OPEN_COUT std::cout.setstate(std::ios_base::goodbit)

class TiXmlElement;
class JObject;

namespace StringUtil {

    /**
     * Returns true if the target string is a substring of the source string
     * @param source std::string; String that can contain the target string
     * @param target std::string; String that can be a substring of the source string
     * @return bool; true if the source string contains the target string
     */
    bool contains(const std::string &source, const std::string &target);

    /**
     * Concatenates s1 and s2 valid result
     * @param s1 the left side of the concatenated string
     * @param s2 the right side of the concatenated string
     * @param result s1 and s2 concatenated
     */
    void concat(const std::string &s1, const std::string &s2, std::string &result);

    /**
     * Concatenates s1 and s2
     * @param s1 the left side of the concatenated string
     * @param s2 the right side of the concatenated string
     * @return s1 and s2 concatenated
     */
    std::string concat(const std::string &s1, const std::string &s2);

    /**
     * Converts a string to an unsigned int
     * @param s the string to convert
     * @param result variable in which to store the converted string
     * @return true if the string is indeed an unsigned int
     */
     bool stringToUnsignedInt(const std::string &s, unsigned int &result);

    /**
    * Converts a string to a double
    * @param s the string to convert
    * @param result variable in which to store the converted string
    * @return true if the string is indeed a double
    */
    bool stringToDouble(const std::string &s, double &result);

}

namespace IntUtil {

    /**
     * Casts an int to a string
     * @param num is the number to be converted
     * @param target is the string that will contain the converted number as a string
     */
    void toString(int num, std::string &target);
}

namespace FileUtil {
    /** ════════════════════════════════════════════================================
    * Name        : TicTacToeUtils.cpp
    * Author      : Serge Demeyer
    * Version     :
    * Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
    * Description : TicTactToe valid C++, Ansi-style
    * Source      : https://github.com/sergedemeyer/TicTacToe_Git/blob/master/TicTacToeUtils.cpp
    * ════════════════════════════════════════════================================*/
    bool DirectoryExists(const std::string& dirname);

    bool FileExists(const std::string &filename);

    bool FileIsEmpty(const std::string &filename);

    bool FileCompare(const std::string& leftFileName, const std::string& rightFileName);
}

namespace TimeUtil {

    /**
     * Copied from https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-valid-c
     * Gets current date and time as a string
     * @param format: string; string format
     * @return current date time as a string
     * @return current date time as a string
     * @return current date time as a string
     */
    std::string getCurrentDateTime(std::string format = "%Y-%m-%d.%X");

}

namespace ParseUtil {
    /**
     * Checks if a given simulation JSON is consistent or not
     * @param parsed: JObject*; simulation as JSON object
     * @return true if the given json object represents a consistent simulation
     * * REQUIRE(parsed, "JSON object cannot be NULL!");
     */
    bool isConsistent(JObject* parsed);
}

#endif //VACCIMULATOR_UTILS_H
