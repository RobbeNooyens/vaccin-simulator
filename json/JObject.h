//
// Created by robbe on 16/03/2021.
//

#ifndef VACCIN_SIMULATOR_JOBJECT_H
#define VACCIN_SIMULATOR_JOBJECT_H

#include <string>
#include <map>

class JValue;

class JObject {
public:
    JObject();

    void put(std::string, JValue*);

    JValue* getValue(const std::string&);

private:
    std::map<std::string, JValue*> values;

};


#endif //VACCIN_SIMULATOR_JOBJECT_H
