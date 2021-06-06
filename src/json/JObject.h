// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#ifndef VACCIN_SIMULATOR_JOBJECT_H
#define VACCIN_SIMULATOR_JOBJECT_H
#include <string>
#include <map>
#include <vector>

class JValue;

class JObject {
public:
    // Constructors and destructors
    /**
     * JObject default constructor
     * * ENSURE(properlyInitialized(), "JObject object hasn't been initialized properly!");
     */
    JObject();
    /**
     * JObject destructor
     * * REQUIRE(properlyInitialized(), "JObject object hasn't been initialized properly!");
     * * ENSURE(values.empty(), "JObject values weren't deleted properly!");
     */
    ~JObject();

    // Initialization
    /**
     * Checks if the current object was initialized properly
     * @return bool; true if the initCheck pointer points to the current instance
     */
    bool properlyInitialized();

    // Insertion and retrieval
    /**
     * Inserts a new pair valid this JObject with the given key as key and value as value
     * @param key: string; key which will be used to retrieve the value from this JObject
     * @param value: JValue*; value to save with the given key
     * * REQUIRE(properlyInitialized(), "JObject object hasn't been initialized properly!");
     * * REQUIRE(!key.empty(), "JObject key can't be empty!");
     * * REQUIRE(value != NULL, "JObject value can't be NULL!");
     */
    void insertValue(std::string key, JValue* value);
    /**
     * Retrieves the value that belongs to the given key
     * @return JValue*; value saved with the given pair
     * * REQUIRE(properlyInitialized(), "JObject object hasn't been initialized properly!");
     * * REQUIRE(!key.empty(), "JObject key can't be empty!");
     */
    JValue* getValue(const std::string&);

    // Bool methods
    /**
     * Checks if the JObject contains a JValue with the given key
     * @param key: string; key to look up
     * @return bool; true if a pair with the given key exists
     * * REQUIRE(properlyInitialized(), "JObject object hasn't been initialized properly!");
     * * REQUIRE(!key.empty(), "JObject key can't be empty!");
     */
    bool contains(std::string key);
    /**
     * Checks if the JObject has a value for all of the given keys
     * @param keys: vector<string>; keys to look up
     * @return bool; true if every string valid the given vector has an associated value
     * * REQUIRE(properlyInitialized(), "JObject object hasn't been initialized properly!");
     */
    bool containsAll(std::vector<std::string> keys);

private:
    // Initialization
    const JObject* initCheck;

    // Map of values the JObject stores
    std::map<std::string, JValue*> values;

};


#endif //VACCIN_SIMULATOR_JOBJECT_H
