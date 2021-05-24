// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛
#include <gtest/gtest.h>
#include "../../src/parsing/XMLParser.h"
#include "../../src/json/JObject.h"
#include "../../src/json/JValue.h"
#include "../../src/json/JArray.h"
#include "../../src/json/JKeys.h"

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
};

// +========================================+
// |          HAPPYDAY SCENARIO'S           |
// +========================================+

/**
 * Tests the default parser
 */
TEST_F(XMLParserTests, ParseDefaultFile) {
    const std::string filename = "tests/data/valid/hub_centers.xml";
    ParseErrors errors;
    std::cerr.flush();
    EXPECT_EQ(0, std::cerr.tellp());
    JObject *parsed = XMLParser::parse(filename, std::cerr, errors);
    EXPECT_EQ(0, std::cerr.tellp());
    EXPECT_TRUE(errors.empty());
    EXPECT_TRUE(parsed->contains(SIMULATION_HUBS) && parsed->contains(SIMULATION_CENTERS));
    // Test hub
    std::vector<JValue*> hubs = parsed->getValue(SIMULATION_HUBS)->asJArray()->getItems();
    EXPECT_EQ((unsigned int)1, hubs.size());
    JObject* hub = hubs.front()->asJObject();
    EXPECT_TRUE(hub->contains(HUB_VACCINES));
    std::vector<JValue *> vaccins = hub->getValue(HUB_VACCINES)->asJArray()->getItems();
    EXPECT_EQ(1, (int) vaccins.size());
    JObject* vaccin = vaccins.front()->asJObject();
    EXPECT_EQ((unsigned int) 93000, vaccin->getValue(VACCINE_DELIVERY)->asUnsignedint());
    EXPECT_EQ((unsigned int) 6, vaccin->getValue(VACCINE_INTERVAL)->asUnsignedint());
    EXPECT_EQ((unsigned int) 2000,vaccin->getValue(VACCINE_TRANSPORTATION)->asUnsignedint());
    std::vector<JValue*> centers = hub->getValue(HUB_CENTERS)->asJArray()->getItems();
    ITERATE(std::vector<JValue*>, centers, center)
        EXPECT_TRUE((*center)->asString() == "Park Spoor Oost" || (*center)->asString() == "AED Studios");
    // Test centra
    EXPECT_EQ((unsigned int) 2, centers.size());
    JObject* center1 = centers.front()->asJObject();
    JObject* center2 = centers.back()->asJObject();
    if(center1->getValue(CENTER_NAME)->asString() != "Park Spoor Oost")
        std::swap(center1, center2);
    EXPECT_TRUE(center1->contains(CENTER_NAME) && center2->contains(CENTER_NAME));
    EXPECT_TRUE(center1->contains(CENTER_ADDRESS) && center2->contains(CENTER_ADDRESS));
    EXPECT_TRUE(center1->contains(CENTER_INHABITANTS) && center2->contains(CENTER_INHABITANTS));
    EXPECT_TRUE(center1->contains(CENTER_CAPACITY) && center2->contains(CENTER_CAPACITY));
    EXPECT_EQ("Park Spoor Oost", center1->getValue(CENTER_NAME)->asString());
    EXPECT_EQ("AED Studios", center2->getValue(CENTER_NAME)->asString());
    EXPECT_EQ("Noordersingel 40, Antwerpen", center1->getValue(CENTER_ADDRESS)->asString());
    EXPECT_EQ("Fabriekstraat 38, Lint", center2->getValue(CENTER_ADDRESS)->asString());
    EXPECT_EQ((unsigned int) 540173, center1->getValue(CENTER_INHABITANTS)->asUnsignedint());
    EXPECT_EQ((unsigned int) 76935, center2->getValue(CENTER_INHABITANTS)->asUnsignedint());
    EXPECT_EQ((unsigned int) 7500, center1->getValue(CENTER_CAPACITY)->asUnsignedint());
    EXPECT_EQ((unsigned int) 2000, center2->getValue(CENTER_CAPACITY)->asUnsignedint());
    // TODO: make parsed being able to be deleted
//    delete parsed;
}

TEST_F(XMLParserTests, ParseVaccinTypes) {
    const std::string filename = "tests/data/valid/hub_vaccine_types.xml";
    ParseErrors errors;
    std::cerr.flush();
    EXPECT_EQ(0, std::cerr.tellp());
    JObject *parsed = XMLParser::parse(filename, std::cerr, errors);
    EXPECT_EQ(0, std::cerr.tellp());
    EXPECT_TRUE(errors.empty());
    EXPECT_TRUE(parsed->contains(SIMULATION_HUBS) && parsed->contains(SIMULATION_CENTERS));
    std::vector<JValue*> hubs = parsed->getValue(SIMULATION_HUBS)->asJArray()->getItems();
    EXPECT_EQ((unsigned int) 1, hubs.size());
    JObject* hub = hubs.front()->asJObject();
    EXPECT_TRUE(hub->contains(HUB_VACCINES));
    std::vector<JValue*> vaccins = hub->getValue(HUB_VACCINES)->asJArray()->getItems();
    std::set<std::string> checked;
    ITERATE(std::vector<JValue*>, vaccins, jVaccin) {
        JObject* vaccin = (*jVaccin)->asJObject();
        EXPECT_TRUE(vaccin->contains(VACCINE_TYPE));
        EXPECT_TRUE(vaccin->contains(VACCINE_DELIVERY));
        EXPECT_TRUE(vaccin->contains(VACCINE_INTERVAL));
        EXPECT_TRUE(vaccin->contains(VACCINE_TRANSPORTATION));
        std::string type = vaccin->getValue(VACCINE_TYPE)->asString();
        // Check if the json contains different vaccins instead of 1 vaccin multiple times
        EXPECT_TRUE(checked.find(type) == checked.end());
        checked.insert(type);
        // Load values to verify
        unsigned int levering = vaccin->getValue(VACCINE_DELIVERY)->asUnsignedint();
        unsigned int interval = vaccin->getValue(VACCINE_INTERVAL)->asUnsignedint();
        unsigned int transport = vaccin->getValue(VACCINE_TRANSPORTATION)->asUnsignedint();
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
    const std::string filename = "tests/data/valid/hub_renewing.xml";
    ParseErrors errors;
    std::cerr.flush();
    EXPECT_EQ(0, std::cerr.tellp());
    JObject *parsed = XMLParser::parse(filename, std::cerr, errors);
    EXPECT_EQ(0, std::cerr.tellp());
    EXPECT_TRUE(errors.empty());
    EXPECT_TRUE(parsed->contains(SIMULATION_HUBS) && parsed->contains(SIMULATION_CENTERS));
    std::vector<JValue*> hubs = parsed->getValue(SIMULATION_HUBS)->asJArray()->getItems();
    EXPECT_EQ((unsigned int) 1, hubs.size());
    JObject* hub = hubs.front()->asJObject();
    EXPECT_TRUE(hub->contains(HUB_VACCINES));
    std::vector<JValue*> vaccins = hub->getValue(HUB_VACCINES)->asJArray()->getItems();
    ITERATE(std::vector<JValue*>, vaccins, jVaccin) {
        JObject* vaccin = (*jVaccin)->asJObject();
        EXPECT_TRUE(vaccin->contains(VACCINE_TYPE));
        EXPECT_TRUE(vaccin->contains(VACCINE_RENEWING));
        std::string type = vaccin->getValue(VACCINE_TYPE)->asString();
        unsigned int renewing = vaccin->getValue(VACCINE_RENEWING)->asUnsignedint();
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
    const std::string filename = "tests/data/valid/hub_temperature.xml";
    ParseErrors errors;
    std::cerr.flush();
    EXPECT_EQ(0, std::cerr.tellp());
    JObject *parsed = XMLParser::parse(filename, std::cerr, errors);
    EXPECT_EQ(0, std::cerr.tellp());
    EXPECT_TRUE(errors.empty());
    EXPECT_TRUE(parsed->contains(SIMULATION_HUBS) && parsed->contains(SIMULATION_CENTERS));
    std::vector<JValue*> hubs = parsed->getValue(SIMULATION_HUBS)->asJArray()->getItems();
    EXPECT_EQ((unsigned int) 1, hubs.size());
    JObject* hub = hubs.front()->asJObject();
    EXPECT_TRUE(hub->contains(HUB_VACCINES));
    std::vector<JValue*> vaccins = hub->getValue(HUB_VACCINES)->asJArray()->getItems();
    ITERATE(std::vector<JValue*>, vaccins, jVaccin) {
        JObject* vaccin = (*jVaccin)->asJObject();
        EXPECT_TRUE(vaccin->contains(VACCINE_TYPE));
        EXPECT_TRUE(vaccin->contains(VACCINE_TEMPERATURE));
        std::string type = vaccin->getValue(VACCINE_TYPE)->asString();
        double temperatuur = vaccin->getValue(VACCINE_TEMPERATURE)->asDouble();
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
    const std::string filename = "tests/data/valid/multiple_hubs.xml";
    ParseErrors errors;
    std::cerr.flush();
    EXPECT_EQ(0, std::cerr.tellp());
    JObject *parsed = XMLParser::parse(filename, std::cerr, errors);
    EXPECT_EQ(0, std::cerr.tellp());
    EXPECT_TRUE(errors.empty());
    EXPECT_TRUE(parsed->contains(SIMULATION_HUBS) && parsed->contains(SIMULATION_CENTERS));
    std::vector<JValue*> hubs = parsed->getValue(SIMULATION_HUBS)->asJArray()->getItems();
    EXPECT_EQ((unsigned int) 2, hubs.size());
    JObject* hub1 = hubs.front()->asJObject();
    JObject* hub2 = hubs.back()->asJObject();
    EXPECT_TRUE(hub1->contains(HUB_VACCINES));
    EXPECT_TRUE(hub2->contains(HUB_VACCINES));
    EXPECT_TRUE(hub1->contains(SIMULATION_CENTERS));
    EXPECT_TRUE(hub2->contains(SIMULATION_CENTERS));
    EXPECT_EQ((unsigned int) 2, hub1->getValue(HUB_VACCINES)->asJArray()->getItems().size());
    EXPECT_EQ((unsigned int) 2, hub2->getValue(HUB_VACCINES)->asJArray()->getItems().size());
    EXPECT_EQ((unsigned int) 2, hub1->getValue(SIMULATION_CENTERS)->asJArray()->getItems().size());
    EXPECT_EQ((unsigned int) 2, hub2->getValue(SIMULATION_CENTERS)->asJArray()->getItems().size());
//    delete parsed;
}

// +========================================+
// |        EXCEPTIONS AND ERRORS           |
// +========================================+

TEST_F(XMLParserTests, NoRootElement) {
    const std::string filename = "tests/data/exceptions/no_root.xml";
    ParseErrors errors;
    std::cerr.flush();
    EXPECT_EQ(0, std::cerr.tellp());
    XMLParser::parse(filename, std::cerr, errors);
    EXPECT_TRUE(std::cerr.tellp() > 0);
    EXPECT_EQ(1, errors.size());
    EXPECT_TRUE(errors.front() == NO_ROOT);
    std::cerr.flush();
}

TEST_F(XMLParserTests, ParseUnknownElement) {
    const std::string filename = "tests/data/exceptions/unknown_element.xml";
    ParseErrors errors;
    std::cerr.flush();
    EXPECT_EQ(0, std::cerr.tellp());
    XMLParser::parse(filename, std::cerr, errors);
    EXPECT_TRUE(std::cerr.tellp() > 0);
    EXPECT_EQ(3, errors.size());
    ITERATE(ParseErrors, errors, error) {
        EXPECT_TRUE(*error == UNKOWN_ELEMENT);
    }
}

TEST_F(XMLParserTests, ParseInvalidInfo) {
    const std::string filename = "tests/data/exceptions/invalid_type.xml";
    ParseErrors errors;
    std::cerr.flush();
    EXPECT_EQ(0, std::cerr.tellp());
    XMLParser::parse(filename, std::cerr, errors);
    EXPECT_TRUE(std::cerr.tellp() > 0);
    EXPECT_EQ(3, errors.size());
    ITERATE(ParseErrors, errors, error) {
        EXPECT_TRUE(*error == INVALID_TYPE);
    }
}

TEST_F(XMLParserTests, EmptyElements) {
    const std::string filename = "tests/data/exceptions/empty_elements.xml";
    ParseErrors errors;
    std::cerr.flush();
    EXPECT_EQ(0, std::cerr.tellp());
    XMLParser::parse(filename, std::cerr, errors);
    EXPECT_TRUE(std::cerr.tellp() > 0);
    EXPECT_EQ(5, errors.size());
    ITERATE(ParseErrors, errors, error) {
        EXPECT_TRUE(*error == EMPTY_ELEMENT);
    }
}

TEST_F(XMLParserTests, ParseIncosistentSimulation) {
    const std::string filename = "tests/data/exceptions/inconsistent_simulation.xml";
    ParseErrors errors;
    std::cerr.flush();
    EXPECT_EQ(0, std::cerr.tellp());
    XMLParser::parse(filename, std::cerr, errors);
    EXPECT_TRUE(std::cerr.tellp() > 0);
    EXPECT_EQ(1, errors.size());
    ITERATE(ParseErrors, errors, error) {
        EXPECT_TRUE(*error == INCONSISTENT_SIMULATION);
    }
}

TEST_F(XMLParserTests, DuplicatedElements) {
    const std::string filename = "tests/data/exceptions/duplicated_elements.xml";
    ParseErrors errors;
    std::cerr.flush();
    EXPECT_EQ(0, std::cerr.tellp());
    XMLParser::parse(filename, std::cerr, errors);
    EXPECT_TRUE(std::cerr.tellp() > 0);
    EXPECT_EQ(3, errors.size());
    ITERATE(ParseErrors, errors, error) {
        EXPECT_TRUE(*error == DUPLICATED_ELEMENT);
    }
}

TEST_F(XMLParserTests, MissingElements) {
    const std::string filename = "tests/data/exceptions/missing_elements.xml";
    ParseErrors errors;
    std::cerr.flush();
    EXPECT_EQ(0, std::cerr.tellp());
    XMLParser::parse(filename, std::cerr, errors);
    EXPECT_TRUE(std::cerr.tellp() > 0);
    EXPECT_EQ(3, errors.size());
    ITERATE(ParseErrors, errors, error) {
        EXPECT_TRUE(*error == MISSING_ELEMENT);
    }
}

TEST_F(XMLParserTests, MultipleErrors) {
    const std::string filename = "tests/data/exceptions/multiple_errors.xml";
    ParseErrors errors;
    std::cerr.flush();
    EXPECT_EQ(0, std::cerr.tellp());
    XMLParser::parse(filename, std::cerr, errors);
    EXPECT_TRUE(std::cerr.tellp() > 0);
    EXPECT_EQ(4, errors.size());
    EXPECT_TRUE(errors[0] == EMPTY_ELEMENT);
    EXPECT_TRUE(errors[1] == DUPLICATED_ELEMENT);
    EXPECT_TRUE(errors[2] == INVALID_TYPE);
    EXPECT_TRUE(errors[3] == UNKOWN_ELEMENT);
}