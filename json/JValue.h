//
// Created by robbe on 16/03/2021.
//

#ifndef VACCIN_SIMULATOR_JVALUE_H
#define VACCIN_SIMULATOR_JVALUE_H

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
    JValue(JObject);
    JValue(JArray);

    int asInt();
    float asFloat();
    double asDouble();
    char asChar();
    bool asBool();
    std::string asString();

private:
    int valInt;
    float varFloat;
    double valDouble;
    char valChar;
    bool varBool;
    std::string valString;
    JObject varJObject;
    JArray varJArray;


};


#endif //VACCIN_SIMULATOR_JVALUE_H
