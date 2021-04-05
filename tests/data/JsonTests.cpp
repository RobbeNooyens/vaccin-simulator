// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛
#include <gtest/gtest.h>
#include "../../json/JObject.h"
#include "../../json/JValue.h"
#include "../../json/JArray.h"

class JsonTests : public ::testing::Test {
protected:
    JArray array;
    JObject object;
    JValue value;
};

/**
 * Tests the default constructor
*/
TEST_F(JsonTests, DefaultConstructor) {
    EXPECT_TRUE(array.properlyInitialized());
    EXPECT_TRUE(array.getItems().empty());
    EXPECT_TRUE(object.properlyInitialized());
    EXPECT_TRUE(value.properlyInitialized());
}

TEST_F(JsonTests, SettingValues) {
    value = JValue((std::string) "1");
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

}
