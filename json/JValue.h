// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#ifndef VACCIN_SIMULATOR_JVALUE_H
#define VACCIN_SIMULATOR_JVALUE_H

#include <string>

class JObject;
class JArray;

class JValue {
public:
    JValue();
    explicit JValue(int);
    explicit JValue(float);
    explicit JValue(double);
    explicit JValue(char);
    explicit JValue(bool);
    explicit JValue(unsigned int);
    explicit JValue(std::string);
    explicit JValue(JObject*);
    explicit JValue(JArray*);

    bool properlyInitialized();

    int asInt() const;
    unsigned int asUnsignedint() const;
    float asFloat() const;
    double asDouble() const;
    char asChar() const;
    bool asBool() const;
    std::string asString();
    JObject* asJObject();
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


#endif //VACCIN_SIMULATOR_JVALUE_H
