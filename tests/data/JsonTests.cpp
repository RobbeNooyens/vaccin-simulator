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
    JValue value = JValue((std::string) "1");
    EXPECT_TRUE(value.asString() == "1");
    value = JValue((int) 1);
    EXPECT_TRUE(value.asInt() == 1);
    value = JValue((unsigned int) 1);
    EXPECT_TRUE(value.asUnsignedint() == 1);
    value = JValue((bool) true);
    EXPECT_TRUE(value.asBool() == true);
    value = JValue((char) '1');
    EXPECT_TRUE(value.asChar() == '1');
    value = JValue((int) 1.0);
    EXPECT_TRUE(value.asDouble() >= 1.0 && value.asDouble() <= 1.0);
    value = JValue((int) 1.0);
    EXPECT_TRUE(value.asFloat() >= 1.0 && value.asFloat() <= 1.0);
    JObject* jobject = new JObject();
    value = JValue(jobject);
    EXPECT_TRUE(value.asJObject() == jobject);
    delete jobject;
    JArray* jarray = new JArray();
    value = JValue(jobject);
    EXPECT_TRUE(value.asJArray() == jarray);
    delete jarray;
}

/**
 * Tests the happyday scenarios by inserting data in the 3 different datastructures and using and deleting them.
 */
TEST_F(JsonTests, HappyDay) {
    JObject* jObject = new JObject();
    jObject->insertValue("key", new JValue("value"));
    JObject* jObject2 = new JObject();
    jObject->insertValue("key2", new JValue("value2"));
    JArray* jArray = new JArray();
    jArray->insertValue(new JValue(jObject));
    jArray->insertValue(new JValue(jObject2));
    JObject* json = new JObject();
    json->insertValue("data", new JValue(jArray));
    std::vector<JValue*> values = json->getValue("data")->asJArray()->getItems();
    EXPECT_TRUE(values[0]->asJObject()->getValue("key")->asString() == "value");
    EXPECT_TRUE(values[1]->asJObject()->getValue("key2")->asString() == "value2");
    delete json;
    // Test dot operator on nested JObjects
    JObject* inner = new JObject();
    inner->insertValue("3", new JValue("4"));
    JObject* middle = new JObject();
    middle->insertValue("2", new JValue(inner));
    JObject* outer = new JObject();
    outer->insertValue("1", new JValue(middle));
    EXPECT_TRUE(outer->getValue("1.2.3")->asString() == "4");
    // Test deletion
    JArray* deletionTest = new JArray();
    deletionTest->insertValue(new JValue(outer));
    delete deletionTest;
    EXPECT_TRUE(outer == NULL);
    EXPECT_TRUE(middle == NULL);
    EXPECT_TRUE(inner == NULL);
}
