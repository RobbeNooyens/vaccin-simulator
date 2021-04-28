// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#ifndef VACCIN_SIMULATOR_JVALUE_H
#define VACCIN_SIMULATOR_JVALUE_H

#include <string>
#include <vector>

class JObject;
class JArray;

class JValue {
public:
    // Constructors and destructors
    /**
     * JValue default constructor
     * * ENSURE(properlyInitialized(), "JValue object hasn't been initialized properly!");
     */
    JValue();
    /**
     * JValue destructor
     * * REQUIRE(properlyInitialized(), "JValue object hasn't been initialized properly!");
     */
    ~JValue();
    /**
     * Creates a JValue instance and sets the member of the right type to the given value
     * @param value: int; value to store in this JValue
     * * ENSURE(properlyInitialized(), "JValue object hasn't been initialized properly!");
     */
    explicit JValue(int value);
    /**
     * Creates a JValue instance and sets the member of the right type to the given value
     * @param value: float; value to store in this JValue
     * * ENSURE(properlyInitialized(), "JValue object hasn't been initialized properly!");
     */
    explicit JValue(float value);
    /**
     * Creates a JValue instance and sets the member of the right type to the given value
     * @param value: double; value to store in this JValue
     * * ENSURE(properlyInitialized(), "JValue object hasn't been initialized properly!");
     */
    explicit JValue(double value);
    /**
     * Creates a JValue instance and sets the member of the right type to the given value
     * @param value: char; value to store in this JValue
     * * ENSURE(properlyInitialized(), "JValue object hasn't been initialized properly!");
     */
    explicit JValue(char value);
    /**
     * Creates a JValue instance and sets the member of the right type to the given value
     * @param value: bool; value to store in this JValue
     * * ENSURE(properlyInitialized(), "JValue object hasn't been initialized properly!");
     */
    explicit JValue(bool value);
    /**
     * Creates a JValue instance and sets the member of the right type to the given value
     * @param value: unsigned int; value to store in this JValue
     * * ENSURE(properlyInitialized(), "JValue object hasn't been initialized properly!");
     */
    explicit JValue(unsigned int value);
    /**
     * Creates a JValue instance and sets the member of the right type to the given value
     * @param value: string; value to store in this JValue
     * * ENSURE(properlyInitialized(), "JValue object hasn't been initialized properly!");
     */
    explicit JValue(std::string value);
    /**
     * Creates a JValue instance and sets the member of the right type to the given value
     * @param value: JObject*; value to store in this JValue
     * * ENSURE(properlyInitialized(), "JValue object hasn't been initialized properly!");
     */
    explicit JValue(JObject* value);
    /**
     * Creates a JValue instance and sets the member of the right type to the given value
     * @param value: JArray*; value to store in this JValue
     * * ENSURE(properlyInitialized(), "JValue object hasn't been initialized properly!");
     */
    explicit JValue(JArray* value);

    /**
     * Checks if the current object was initialized properly
     * @return bool; true if the initCheck pointer points to the current instance
     */
    bool properlyInitialized() const;

    /**
     * Returns the saved int value
     * @return int; saved int value
     * * REQUIRE(properlyInitialized(), "JValue object hasn't been initialized properly!");
     */
    int asInt() const;
    /**
     * Returns the saved unsigned int value
     * @return unsigned int; saved unsigned int value
     * * REQUIRE(properlyInitialized(), "JValue object hasn't been initialized properly!");
     */
    unsigned int asUnsignedint() const;
    /**
     * Returns the saved float value
     * @return float; saved float value
     * * REQUIRE(properlyInitialized(), "JValue object hasn't been initialized properly!");
     */
    float asFloat() const;
    /**
     * Returns the saved double value
     * @return double; saved double value
     * * REQUIRE(properlyInitialized(), "JValue object hasn't been initialized properly!");
     */
    double asDouble() const;
    /**
     * Returns the saved char value
     * @return char; saved char value
     * * REQUIRE(properlyInitialized(), "JValue object hasn't been initialized properly!");
     */
    char asChar() const;
    /**
     * Returns the saved boolean value
     * @return boolean; saved boolean value
     * * REQUIRE(properlyInitialized(), "JValue object hasn't been initialized properly!");
     */
    bool asBool() const;
    /**
     * Returns the saved string value
     * @return string; saved string value
     * * REQUIRE(properlyInitialized(), "JValue object hasn't been initialized properly!");
     */
    std::string asString();
    /**
     * Returns the saved JObject value
     * @return JObject; saved JObject value
     * * REQUIRE(properlyInitialized(), "JValue object hasn't been initialized properly!");
     */
    JObject* asJObject();
    /**
     * Returns the saved JArray value
     * @return JArray; saved JArray value
     * * REQUIRE(properlyInitialized(), "JValue object hasn't been initialized properly!");
     */
    JArray* asJArray();

private:
    // Initialization
    const JValue* initCheck;

    // Actual value the JValue stores
    int valInt;
    float valFloat;
    double valDouble;
    unsigned int valUnsignedint;
    char valChar;
    bool valBool;
    std::string valString;
    JObject* valJObject;
    JArray* valJArray;
};

typedef std::vector<JValue*> JValues;

#endif //VACCIN_SIMULATOR_JVALUE_H
