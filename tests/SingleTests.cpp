// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#include <iostream>
#include <cassert>
#include <vector>
#include <set>

#include "../src/parsing/XMLParser.h"
#include "../src/json/JObject.h"
#include "../src/json/JKeys.h"
#include "../src/json/JValue.h"
#include "../src/json/JArray.h"
#include "../src/entities/VaccinationCenter.h"

#define EXPECT_TRUE(expression) assert(expression)
#define EXPECT_EQ(lhs, rhs) assert(lhs == rhs)
#define ITERATE(type, iteratable, name) for(type::iterator name = iteratable.begin(); name != iteratable.end(); name++)

void expectErrors(const std::string &fileName, const ParseErrors &expectedErrors) {
    ParseErrors errors;
    EXPECT_TRUE(std::cout.good());
    JObject* parsed = XMLParser::parse(fileName, std::cout, errors);
    EXPECT_TRUE(std::cout.good());
    EXPECT_EQ(expectedErrors.size(), errors.size());
    for(int i = 0; i < (int) expectedErrors.size(); i++) {
        EXPECT_EQ(expectedErrors[i], errors[i]);
    }
    delete parsed;
}

int main() {
    // Test dot operator on nested JObjects
    JObject* inner = new JObject();
    JObject* middle = new JObject();
    middle->insertValue("middle", new JValue(inner));
    JObject* outer = new JObject();
    outer->insertValue("outer", new JValue(middle));
    JArray* deletionTest = new JArray();
    deletionTest->insertValue(new JValue(outer));
    delete deletionTest;
    return 0;
}


