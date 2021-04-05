// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#ifndef VACCIN_SIMULATOR_JOBJECT_H
#define VACCIN_SIMULATOR_JOBJECT_H
#include <string>
#include <map>
#include <vector>

class JValue;

class JObject {
public:
    JObject();
    void insertValue(std::string, JValue*);
    JValue* getValue(const std::string&);
    bool contains(std::string key);
    bool containsAll(std::vector<std::string> keys);

private:
    std::map<std::string, JValue*> values;

};


#endif //VACCIN_SIMULATOR_JOBJECT_H
