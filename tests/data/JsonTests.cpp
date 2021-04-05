// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛
#include <gtest/gtest.h>
#include "../../io/XMLParser.h"
#include "../../json/JObject.h"
#include "../../json/JValue.h"

class JsonTests : public ::testing::Test {
protected:
//    XMLParser parser;
};

///**
// * Tests the default constructor
//*/
//TEST_F(JsonTests, DefaultConstructor) {
//    EXPECT_TRUE(parser.properlyInitialized());
//}
//
///**
// * Tests the parser
// */
//TEST_F(JsonTests, ParseFile) {
//    const std::string filename = "tests/data/in/test1.xml";
//    JObject *returned = parser.parse(filename);
//    EXPECT_TRUE(returned->getValue("hub")->asJObject()->getValue("levering")->asUnsignedint() == 93000);
//    EXPECT_TRUE(returned->getValue("hub")->asJObject()->getValue("interval")->asUnsignedint() == 6);
//    EXPECT_TRUE(returned->getValue("hub")->asJObject()->getValue("transport")->asUnsignedint() == 2000);
//}