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
    // Constructors and destructors
    JArray();
    ~JArray();

    // Initialization
    bool properlyInitialized();

    // Insertion and retrieval
    void insertValue(JValue*);
    std::vector<JValue*>& getItems();

private:
    // Initialization
    JArray* initCheck;

    // Values
    std::vector<JValue*> items;

};


#endif //VACCIN_SIMULATOR_JARRAY_H
