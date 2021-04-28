// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#ifndef VACCIMULATOR_UTILS_H
#define VACCIMULATOR_UTILS_H

#include <fstream>
#include <sys/stat.h>
#include <string>
#include <iostream>
#include <sstream>

#define VECTOR_CONTAINS(vector, element) vector.find(element) != vector.end()
#define CLOSE_COUT std::cout.setstate(std::ios_base::badbit)
#define OPEN_COUT std::cout.setstate(std::ios_base::goodbit)

class TiXmlElement;

namespace StringUtil {

    /**
     * Returns true if the target string is a substring of the source string
     * @param source std::string; String that can contain the target string
     * @param target std::string; String that can be a substring of the source string
     * @return bool; true if the source string contains the target string
     */
    bool contains(const std::string &source, const std::string &target);

    /**
     * Concatenates s1 and s2 in result
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
    /** ============================================================================
    * Name        : TicTacToeUtils.cpp
    * Author      : Serge Demeyer
    * Version     :
    * Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
    * Description : TicTactToe in C++, Ansi-style
    * Source      : https://github.com/sergedemeyer/TicTacToe_Git/blob/master/TicTacToeUtils.cpp
    * ============================================================================*/
    bool DirectoryExists(const std::string dirname);

    bool FileExists(const std::string filename);

    bool FileIsEmpty(const std::string filename);

    bool FileCompare(const std::string leftFileName, const std::string rightFileName);
}

namespace XMLUtil {

    /**
     * Converts an XML element to an unsigned int
     * @param element: TiXmlElement*; element to convert
     * @param elementName: string; element name to print errormessage
     * @return unsigned int; int representatino of the xml element value
     * REQUIRE(element != NULL, "Element can't be NULL!");
     * REQUIRE(!elementName.empty(), "Element name cannot be empty!");
     */
    unsigned int elementToUnsignedInt(TiXmlElement* element, const std::string& elementName);

}

namespace TimeUtil {

    /**
     * Copied from https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
     * Gets current date and time as a string
     * @param format: string; string format
     * @return current date time as a string
     * @return current date time as a string
     * @return current date time as a string
     */
    std::string getCurrentDateTime(std::string format = "%Y-%m-%d.%X");

}

#endif //VACCIMULATOR_UTILS_H
