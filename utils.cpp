// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include "utils.h"
#include "tinyxml/tinyxml.h"
#include "DesignByContract.h"
#include <time.h>

bool StringUtil::contains(const std::string &source, const std::string &target) {
    return source.find(target) != std::string::npos;
}

void StringUtil::concat(const std::string &s1, const std::string &s2, std::string &result) {
    std::stringstream ss;
    ss << s1 << s2;
    result = ss.str();
}

std::string StringUtil::concat(const std::string &s1, const std::string &s2) {
    std::stringstream ss;
    ss << s1 << s2;
    return ss.str();
}

void IntUtil::toString(int num, std::string &target) {
    std::ostringstream convert;   // stream used for the conversion
    convert << num;      // insert the textual representation of 'Number' in the characters in the stream
    target = convert.str();
}

bool FileUtil::DirectoryExists(const std::string dirname) {
    struct stat st;
    return stat(dirname.c_str(), &st) == 0;
}

bool FileUtil::FileExists(const std::string filename) {
    struct stat st;
    if (stat(filename.c_str(), &st) != 0) return false;
    std::ifstream f(filename.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }
}

bool FileUtil::FileIsEmpty(const std::string filename) {
    struct stat st;
    if (stat(filename.c_str(), &st) != 0) return true; // File does not exist; thus it is empty
    return st.st_size == 0;
}

bool FileUtil::FileCompare(const std::string leftFileName, const std::string rightFileName) {
    std::ifstream leftFile, rightFile;
    char leftRead, rightRead;
    bool result;

    // Open the two files.
    leftFile.open(leftFileName.c_str());
    if (!leftFile.is_open()) {
        return false;
    };
    rightFile.open(rightFileName.c_str());
    if (!rightFile.is_open()) {
        leftFile.close();
        return false;
    };

    result = true; // files exist and are open; assume equality unless a counterexamples shows up.
    while (result && leftFile.good() && rightFile.good()) {
        leftFile.get(leftRead);
        rightFile.get(rightRead);
        result = (leftRead == rightRead);
    };
    if (result) {
        // last read was still equal; are we at the end of both files ?
        result = (!leftFile.good()) && (!rightFile.good());
    };

    leftFile.close();
    rightFile.close();
    return result;
}

unsigned int XMLUtil::elementToUnsignedInt(TiXmlElement *element, const std::string& elementName) {
    REQUIRE(element != NULL, "Element can't be NULL!");
    REQUIRE(!elementName.empty(), "Element name cannot be empty!");
    TiXmlText* e_text = element->FirstChild()->ToText();
    if (e_text == NULL) {
        throw std::runtime_error("waarde niet gevonden in element '" + elementName + "'");
    }
    char *ptr;
    std::string value = e_text->Value();
    unsigned int k = strtoul(value.c_str(), &ptr, 10);
    if (value.c_str() == ptr) {
        throw std::runtime_error("waarde kon niet ingelezen worden van element '" + elementName + "'");
    }
    return k;
}

std::string TimeUtil::getCurrentDateTime(std::string format) {
    time_t     now = time(0);
    struct tm  tstruct = {};
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), format.c_str(), &tstruct);
    return buf;
}
