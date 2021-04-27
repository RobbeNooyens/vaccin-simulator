// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛
#include <gtest/gtest.h>
#include "../../json/JObject.h"
#include "../../json/JValue.h"
#include "../../json/JArray.h"

class JsonTests : public ::testing::Test {
};

/**
 * Tests the default constructor
*/
TEST_F(JsonTests, DefaultConstructor) {
    JArray array = JArray();
    JObject object = JObject();
    JValue value = JValue();
    EXPECT_TRUE(array.properlyInitialized());
    EXPECT_TRUE(array.getItems().empty());
    EXPECT_TRUE(object.properlyInitialized());
    EXPECT_TRUE(value.properlyInitialized());
}

/**
 * Tests setting and retrieving values in JValue objects
 */
TEST_F(JsonTests, SettingValues) {
    EXPECT_TRUE(JValue((std::string) "1").asString() == "1");
    EXPECT_TRUE(JValue((int) 1).asInt() == 1);
    EXPECT_TRUE(JValue((unsigned int) 1).asUnsignedint() == 1);
    EXPECT_TRUE(JValue((bool) true).asBool() == true);
    EXPECT_TRUE(JValue((char) '1').asChar() == '1');
    EXPECT_TRUE(std::abs(JValue((double) 1.0).asDouble() - 1) < 0.001);
    EXPECT_TRUE(std::abs(JValue((float) 1.0).asFloat() - 1) < 0.001);
    JObject* jobject = new JObject();
    EXPECT_TRUE(JValue(jobject).asJObject() == jobject);
    JArray* jarray = new JArray();
    EXPECT_TRUE(JValue(jarray).asJArray() == jarray);
}

/**
 * Tests the happyday scenarios by inserting data in the 3 different datastructures and using and deleting them.
 */
TEST_F(JsonTests, HappyDay) {
    JObject* jObject = new JObject();
    jObject->insertValue("key", new JValue((std::string) "value"));
    JObject* jObject2 = new JObject();
    jObject2->insertValue("key2", new JValue((std::string) "value2"));
    JArray* jArray = new JArray();
    jArray->insertValue(new JValue(jObject));
    jArray->insertValue(new JValue(jObject2));
    JObject* json = new JObject();
    json->insertValue("data", new JValue(jArray));
    std::vector<JValue*> values = json->getValue("data")->asJArray()->getItems();
    EXPECT_EQ("value", values[0]->asJObject()->getValue("key")->asString());
    EXPECT_EQ("value2", values[1]->asJObject()->getValue("key2")->asString());
    delete json;
}

TEST_F(JsonTests, DotOperator) {
    // Test dot operator on nested JObjects
    JObject* inner = new JObject();
    inner->insertValue("3", new JValue((std::string) "4"));
    JObject* middle = new JObject();
    middle->insertValue("2", new JValue(inner));
    JObject* outer = new JObject();
    outer->insertValue("1", new JValue(middle));
    EXPECT_TRUE(outer->getValue("1.2.3")->asString() == "4");
    delete outer;
}

TEST_F(JsonTests, NestedDeletion) {
    // Test dot operator on nested JObjects
    JObject* inner = new JObject();
    JObject* middle = new JObject();
    middle->insertValue("", new JValue(inner));
    JObject* outer = new JObject();
    outer->insertValue("", new JValue(middle));
    JArray* deletionTest = new JArray();
    deletionTest->insertValue(new JValue(outer));
    EXPECT_NO_FATAL_FAILURE(delete deletionTest);
}