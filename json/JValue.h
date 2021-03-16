//
// Created by robbe on 16/03/2021.
//

#ifndef VACCIN_SIMULATOR_JVALUE_H
#define VACCIN_SIMULATOR_JVALUE_H

#include <string>

class JObject;
class JArray;

class JValue {
public:
    JValue(int);
    JValue(float);
    JValue(double);
    JValue(char);
    JValue(bool);
    JValue(std::string);
    JValue(JObject*);
    JValue(JArray*);

    int asInt() const;
    float asFloat() const;
    double asDouble() const;
    char asChar() const;
    bool asBool() const;
    std::string asString();
    JObject* asJObject();
    JArray* asJArray();

private:
    int valInt;
    float valFloat;
    double valDouble;
    char valChar;
    bool valBool;
    std::string valString;
    JObject* valJObject;
    JArray* valJArray;
};


#endif //VACCIN_SIMULATOR_JVALUE_H
