// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛
#ifndef VACCIN_SIMULATOR_JARRAY_H
#define VACCIN_SIMULATOR_JARRAY_H

#include <vector>

class JValue;

class JArray {
public:
    JArray();
    ~JArray();
    void insertValue(JValue*);
    std::vector<JValue*>& getItems();

private:
    std::vector<JValue*> items;

};


#endif //VACCIN_SIMULATOR_JARRAY_H
