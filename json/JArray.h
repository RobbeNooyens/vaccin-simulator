// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛
#ifndef VACCIN_SIMULATOR_JARRAY_H
#define VACCIN_SIMULATOR_JARRAY_H

#include <vector>

class JValue;

class JArray {
public:
    // Constructors and destructors
    /**
     * JArray default constructor
     * ENSURE(properlyInitialized(), "JArray object hasn't been initialized properly!");
     */
    JArray();
    /**
     * REQUIRE(properlyInitialized(), "JArray object hasn't been initialized properly!");
     */
    ~JArray();

    // Initialization
    /**
     * Checks if the current object was initialized properly
     * @return bool; true if the initCheck pointer points to the current instance
     */
    bool properlyInitialized();

    // Insertion and retrieval
    /**
     * Inserts a JValue in the array
     * @param value: JValue*; value to insert
     * REQUIRE(properlyInitialized(), "JArray object hasn't been initialized properly!");
     */
    void insertValue(JValue* value);
    /**
     * Retrieves all values saved in this array
     * @return std::vector<JValue*>; vector of the JValues stored in the array
     * REQUIRE(properlyInitialized(), "JArray object hasn't been initialized properly!");
     */
    std::vector<JValue*>& getItems();

private:
    // Initialization
    const JArray* initCheck;

    // Values
    std::vector<JValue*> items;

};


#endif //VACCIN_SIMULATOR_JARRAY_H
