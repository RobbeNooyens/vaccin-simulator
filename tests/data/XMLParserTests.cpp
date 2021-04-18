// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛
#include <gtest/gtest.h>
#include "../../io/XMLParser.h"
#include "../../json/JObject.h"
#include "../../json/JValue.h"

class XMLParserTests : public ::testing::Test {
protected:
    XMLParser parser;
};

/**
 * Tests the default constructor
*/
TEST_F(XMLParserTests, DefaultConstructor) {
    EXPECT_TRUE(parser.properlyInitialized());
}

/**
 * Tests the parser
 */
TEST_F(XMLParserTests, ParseDefaultFile) {
    const std::string filename = "tests/data/in/hub_with_centers.xml";
    JObject *returned = parser.parse(filename);
    EXPECT_TRUE(returned->getValue("hub")->asJObject()->getValue("levering")->asUnsignedint() == 93000);
    EXPECT_TRUE(returned->getValue("hub")->asJObject()->getValue("interval")->asUnsignedint() == 6);
    EXPECT_TRUE(returned->getValue("hub")->asJObject()->getValue("transport")->asUnsignedint() == 2000);
}

TEST_F(XMLParserTests, ParseVaccinTypes) {

}

TEST_F(XMLParserTests, ParseVaccinRenewings) {

}

TEST_F(XMLParserTests, ParseVaccinTemperatures) {

}

TEST_F(XMLParserTests, ParseMultipleHubs) {

}

TEST_F(XMLParserTests, ParseFileWithUnknownElement) {

}

TEST_F(XMLParserTests, ParseFileWithInvalidInfo) {

}

TEST_F(XMLParserTests, ParseFileWithIncosistentSimulation) {

}