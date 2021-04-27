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
#include "../../json/JArray.h"
// TODO: replace strings with JKeys
#include "../../json/JKeys.h"

#define ITERATE(type, iteratable, name) for(type::iterator name = iteratable.begin(); name != iteratable.end(); name++)

/*
 * hubs:
 *   hubX:
 *     vaccins:
 *       vaccinX:
 *         type: ""
 *         levering: 0
 *         interval: 0
 *         transport: 0
 *         hernieuwing: 0
 *         temperatuur: 0.0
 *     interval: 0
 *     levering: 0
 *     transport: 0
 *     centra:
 *       - ""
 * centra:
 *   centraX:
 *     naam: ""
 *     adres: ""
 *     inwoners: 0
 *     capaciteit: 0
 */

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
 * Tests the default parser
 */
TEST_F(XMLParserTests, ParseDefaultFile) {
    const std::string filename = "tests/data/in/hub_centers.xml";
    JObject *parsed = parser.parse(filename);
    EXPECT_TRUE(parsed->contains("hubs") && parsed->contains("centra"));
    // Test hub
    std::vector<JValue*> hubs = parsed->getValue("hubs")->asJArray()->getItems();
    EXPECT_EQ((unsigned int)1, hubs.size());
    JObject* hub = hubs.front()->asJObject();
    EXPECT_TRUE(hub->contains("vaccins"));
    std::vector<JValue *> vaccins = hub->getValue("vaccins")->asJArray()->getItems();
    EXPECT_EQ(1, (int) vaccins.size());
    JObject* vaccin = vaccins.front()->asJObject();
    EXPECT_EQ((unsigned int) 93000, vaccin->getValue("levering")->asUnsignedint());
    EXPECT_EQ((unsigned int) 6, vaccin->getValue("interval")->asUnsignedint());
    EXPECT_EQ((unsigned int) 2000,vaccin->getValue("transport")->asUnsignedint());
    std::vector<JValue*> centers = hub->getValue("centra")->asJArray()->getItems();
    ITERATE(std::vector<JValue*>, centers, center)
        EXPECT_TRUE((*center)->asString() == "Park Spoor Oost" || (*center)->asString() == "AED Studios");
    // Test centra
    std::vector<JValue*> centra = parsed->getValue("centra")->asJArray()->getItems();
    EXPECT_EQ((unsigned int) 2, centra.size());
    JObject* center1 = centra.front()->asJObject();
    JObject* center2 = centra.back()->asJObject();
    if(center1->getValue("naam")->asString() != "Park Spoor Oost")
        std::swap(center1, center2);
    EXPECT_TRUE(center1->contains("naam") && center2->contains("naam"));
    EXPECT_TRUE(center1->contains("adres") && center2->contains("adres"));
    EXPECT_TRUE(center1->contains("inwoners") && center2->contains("inwoners"));
    EXPECT_TRUE(center1->contains("capaciteit") && center2->contains("capaciteit"));
    EXPECT_EQ("Park Spoor Oost", center1->getValue("naam")->asString());
    EXPECT_EQ("AED Studios", center2->getValue("naam")->asString());
    EXPECT_EQ("Noordersingel 40, Antwerpen", center1->getValue("adres")->asString());
    EXPECT_EQ("Fabriekstraat 38, Lint", center2->getValue("adres")->asString());
    EXPECT_EQ((unsigned int) 540173, center1->getValue("inwoners")->asUnsignedint());
    EXPECT_EQ((unsigned int) 76935, center2->getValue("inwoners")->asUnsignedint());
    EXPECT_EQ((unsigned int) 7500, center1->getValue("capaciteit")->asUnsignedint());
    EXPECT_EQ((unsigned int) 2000, center2->getValue("capaciteit")->asUnsignedint());
    // TODO: make parsed being able to be deleted
//    delete parsed;
}

TEST_F(XMLParserTests, ParseVaccinTypes) {
    const std::string filename = "tests/data/in/hub_vaccine_types.xml";
    JObject *parsed = parser.parse(filename);
    EXPECT_TRUE(parsed->contains("hubs") && parsed->contains("centra"));
    std::vector<JValue*> hubs = parsed->getValue("hubs")->asJArray()->getItems();
    EXPECT_EQ((unsigned int) 1, hubs.size());
    JObject* hub = hubs.front()->asJObject();
    EXPECT_TRUE(hub->contains("vaccins"));
    std::vector<JValue*> vaccins = hub->getValue("vaccins")->asJArray()->getItems();
    std::set<std::string> checked;
    ITERATE(std::vector<JValue*>, vaccins, jVaccin) {
        JObject* vaccin = (*jVaccin)->asJObject();
        EXPECT_TRUE(vaccin->contains("type"));
        EXPECT_TRUE(vaccin->contains("levering"));
        EXPECT_TRUE(vaccin->contains("interval"));
        EXPECT_TRUE(vaccin->contains("transport"));
        std::string type = vaccin->getValue("type")->asString();
        // Check if the json contains different vaccins instead of 1 vaccin multiple times
        EXPECT_TRUE(checked.find(type) == checked.end());
        checked.insert(type);
        // Load values to verify
        unsigned int levering = vaccin->getValue("levering")->asUnsignedint();
        unsigned int interval = vaccin->getValue("interval")->asUnsignedint();
        unsigned int transport = vaccin->getValue("transport")->asUnsignedint();
        EXPECT_TRUE(type == "Pfizer" || type == "Moderna" || type == "AstraZeneca");
        if(type == "Pfizer") {
            EXPECT_EQ((unsigned int) 93000, levering);
            EXPECT_EQ((unsigned int) 6, interval);
            EXPECT_EQ((unsigned int) 2000, transport);
        } else if(type == "Moderna") {
            EXPECT_EQ((unsigned int) 46000, levering);
            EXPECT_EQ((unsigned int) 13, interval);
            EXPECT_EQ((unsigned int) 1000, transport);
        } else if(type == "AstraZeneca") {
            EXPECT_EQ((unsigned int) 67000, levering);
            EXPECT_EQ((unsigned int) 4, interval);
            EXPECT_EQ((unsigned int) 1500, transport);
        }
    }
    // TODO: deletion
//    delete parsed;
}

TEST_F(XMLParserTests, ParseVaccinRenewings) {
    const std::string filename = "tests/data/in/hub_renewing.xml";
    JObject *parsed = parser.parse(filename);
    EXPECT_TRUE(parsed->contains("hubs") && parsed->contains("centra"));
    std::vector<JValue*> hubs = parsed->getValue("hubs")->asJArray()->getItems();
    EXPECT_EQ((unsigned int) 1, hubs.size());
    JObject* hub = hubs.front()->asJObject();
    EXPECT_TRUE(hub->contains("vaccins"));
    std::vector<JValue*> vaccins = hub->getValue("vaccins")->asJArray()->getItems();
    ITERATE(std::vector<JValue*>, vaccins, jVaccin) {
        JObject* vaccin = (*jVaccin)->asJObject();
        EXPECT_TRUE(vaccin->contains("type"));
        EXPECT_TRUE(vaccin->contains("hernieuwing"));
        std::string type = vaccin->getValue("type")->asString();
        unsigned int renewing = vaccin->getValue("hernieuwing")->asUnsignedint();
        EXPECT_TRUE(type == "Pfizer" || type == "Moderna" || type == "AstraZeneca");
        if(type == "Pfizer") {
            EXPECT_EQ((unsigned int) 21, renewing);
        } else if(type == "Moderna") {
            EXPECT_EQ((unsigned int) 28, renewing);
        } else if(type == "AstraZeneca") {
            EXPECT_EQ((unsigned int) 28, renewing);
        }
    }
    // TODO: deletion
//    delete parsed;
}

TEST_F(XMLParserTests, ParseVaccinTemperatures) {
    const std::string filename = "tests/data/in/hub_temperature.xml";
    JObject *parsed = parser.parse(filename);
    EXPECT_TRUE(parsed->contains("hubs") && parsed->contains("centra"));
    std::vector<JValue*> hubs = parsed->getValue("hubs")->asJArray()->getItems();
    EXPECT_EQ((unsigned int) 1, hubs.size());
    JObject* hub = hubs.front()->asJObject();
    EXPECT_TRUE(hub->contains("vaccins"));
    std::vector<JValue*> vaccins = hub->getValue("vaccins")->asJArray()->getItems();
    ITERATE(std::vector<JValue*>, vaccins, jVaccin) {
        JObject* vaccin = (*jVaccin)->asJObject();
        EXPECT_TRUE(vaccin->contains("type"));
        EXPECT_TRUE(vaccin->contains("temperatuur"));
        std::string type = vaccin->getValue("type")->asString();
        double temperatuur = vaccin->getValue("temperatuur")->asDouble();
        EXPECT_TRUE(type == "Pfizer" || type == "Moderna" || type == "AstraZeneca");
        if(type == "Pfizer") {
            EXPECT_EQ((double) -70, temperatuur);
        } else if(type == "Moderna") {
            EXPECT_EQ((double) -20, temperatuur);
        } else if(type == "AstraZeneca") {
            EXPECT_EQ((double) 5, temperatuur);
        }
    }
//    delete parsed;
}

TEST_F(XMLParserTests, ParseMultipleHubs) {
    const std::string filename = "tests/data/in/multiple_hubs.xml";
    JObject *parsed = parser.parse(filename);
    EXPECT_TRUE(parsed->contains("hubs") && parsed->contains("centra"));
    std::vector<JValue*> hubs = parsed->getValue("hubs")->asJArray()->getItems();
    EXPECT_EQ((unsigned int) 2, hubs.size());
    JObject* hub1 = hubs.front()->asJObject();
    JObject* hub2 = hubs.back()->asJObject();
    EXPECT_TRUE(hub1->contains("vaccins"));
    EXPECT_TRUE(hub2->contains("vaccins"));
    EXPECT_TRUE(hub1->contains("centra"));
    EXPECT_TRUE(hub2->contains("centra"));
    EXPECT_EQ((unsigned int) 2, hub1->getValue("vaccins")->asJArray()->getItems().size());
    EXPECT_EQ((unsigned int) 2, hub2->getValue("vaccins")->asJArray()->getItems().size());
    EXPECT_EQ((unsigned int) 2, hub1->getValue("centra")->asJArray()->getItems().size());
    EXPECT_EQ((unsigned int) 2, hub2->getValue("centra")->asJArray()->getItems().size());
//    delete parsed;
}

TEST_F(XMLParserTests, ParseUnknownElement) {
    const std::string filename = "tests/data/in/exception_unknown_element.xml";
    // TODO: Change parser algorithm to intercept invalid tags
//    EXPECT_DEATH(parser.parse(filename), "Attribute oppervlakte shouldn't be parsed!");
}

TEST_F(XMLParserTests, ParseInvalidInfo) {
    const std::string filename = "tests/data/in/exception_invalid_information.xml";
    // TODO: Find out why this doesn't work
//    EXPECT_DEATH(parser.parse(filename), ".*waarde kon niet ingelezen worden.*");
}

//TEST_F(XMLParserTests, ParseIncosistentSimulation) {
//    const std::string filename = "tests/data/in/exception_inconsistent_simulation.xml";
//    EXPECT_DEATH(parser.parse(filename), "Simulation should be inconsistent");
//}