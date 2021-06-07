// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#include "utils.h"
#include "../../tinyxml/tinyxml.h"
#include "../../DesignByContract.h"
#include "../json/JValue.h"
#include "../json/JObject.h"
#include "../json/JArray.h"
#include "../json/JKeys.h"
#include <ctime>
#include <vector>
#include <set>

#define ITERATE(type, iteratable, name) for(type::iterator name = iteratable.begin(); name != iteratable.end(); name++)

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

bool StringUtil::stringToUnsignedInt(const std::string &s, unsigned int &result) {
    // Cast to double first to avoid doubles being parsed as unsigned integers
    double d;
    if(!stringToDouble(s, d))
        return false;
    result = (unsigned int) d;
    return (double) result == d;
}

bool StringUtil::stringToDouble(const std::string &s, double &result) {
    char *ptr;
    result = std::strtod(s.c_str(), &ptr);
    return s.c_str() != ptr;
}

void IntUtil::toString(int num, std::string &target) {
    std::ostringstream convert;   // stream used for the conversion
    convert << num;      // insert the textual representation of 'Number' valid the characters valid the stream
    target = convert.str();
}

bool FileUtil::DirectoryExists(const std::string& dirname) {
    struct stat st;
    return stat(dirname.c_str(), &st) == 0;
}

bool FileUtil::FileExists(const std::string& filename) {
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

bool FileUtil::FileIsEmpty(const std::string& filename) {
    struct stat st;
    if (stat(filename.c_str(), &st) != 0) return true; // File does not exist; thus it is empty
    return st.st_size == 0;
}

bool FileUtil::FileCompare(const std::string& leftFileName, const std::string& rightFileName) {
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

bool ParseUtil::isConsistent(JObject *parsed) {
    REQUIRE(parsed, "JSON object cannot be NULL!");
    // There should be a hubs and centers element to be consistent
    if(!parsed->contains(SIMULATION_HUBS) || !parsed->contains(SIMULATION_CENTERS))
        return false;

    JValues hubs = parsed->getValue(SIMULATION_HUBS)->asJArray()->getItems();
    JValues centers = parsed->getValue(SIMULATION_CENTERS)->asJArray()->getItems();
    std::set<std::string> centerNames;

    // There's at least one hub
    if(hubs.empty())
        return false;

    // Every hub has at least one vaccine being delivered
    ITERATE(JValues, hubs, hubValue) {
        JObject* hub = (*hubValue)->asJObject();
        if(!hub->contains(HUB_VACCINES))
            return false;
        JValues vaccines = hub->getValue(HUB_VACCINES)->asJArray()->getItems();
        if(vaccines.empty())
            return false;
        bool hasVaccine = false;
        ITERATE(JValues, vaccines, vaccineValue) {
            JObject* vaccine = (*vaccineValue)->asJObject();
            if(!vaccine->contains(VACCINE_INTERVAL))
                continue;
            if(vaccine->getValue(VACCINE_INTERVAL)->asUnsignedint() > 0) {
                hasVaccine = true;
                break;
            }
        }
        if(!hasVaccine)
            return false;
    }

    // There's at least one center
    if(centers.empty())
        return false;

    // Every hub is connected to at least one center
    ITERATE(JValues, hubs, hubValue) {
        JObject* hub = (*hubValue)->asJObject();
        if(!hub->contains(HUB_CENTERS))
            return false;
        JValues hubCenters = hub->getValue(HUB_CENTERS)->asJArray()->getItems();
        if(hubCenters.empty())
            return false;
        ITERATE(JValues, hubCenters, hubCenterValue) {
            std::string hubCenter = (*hubCenterValue)->asString();
            centerNames.insert(hubCenter);
        }
    }

    // Every center is connected to at least one hub
    std::set<JObject*> usedCenters;
    ITERATE(JValues, centers, centerValue) {
        JObject* center = (*centerValue)->asJObject();
        if(!center->contains(CENTER_NAME))
            return false;
        if(centerNames.find(center->getValue(CENTER_NAME)->asString()) == centerNames.end())
            return false;
        usedCenters.insert(center);
    }

    // Every center in a hub matches a valid center
    if(centerNames.size() != usedCenters.size())
        return false;

    return true;
}
