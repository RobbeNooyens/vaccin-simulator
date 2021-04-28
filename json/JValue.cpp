// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include "JValue.h"
#include "JObject.h"
#include "JArray.h"
#include "../DesignByContract.h"

JValue::JValue(): initCheck(this), valJObject(NULL), valJArray(NULL) {
    ENSURE(properlyInitialized(), "JValue object hasn't been initialized properly!");
}
JValue::JValue(int val): initCheck(this), valInt(val), valJObject(NULL), valJArray(NULL){
    ENSURE(properlyInitialized(), "JValue object hasn't been initialized properly!");
}
JValue::JValue(float val): initCheck(this), valFloat(val), valJObject(NULL), valJArray(NULL){
    ENSURE(properlyInitialized(), "JValue object hasn't been initialized properly!");
}
JValue::JValue(double val): initCheck(this), valDouble(val), valJObject(NULL), valJArray(NULL){
    ENSURE(properlyInitialized(), "JValue object hasn't been initialized properly!");
}
JValue::JValue(char val): initCheck(this), valChar(val), valJObject(NULL), valJArray(NULL){
    ENSURE(properlyInitialized(), "JValue object hasn't been initialized properly!");
}
JValue::JValue(bool val): initCheck(this), valBool(val), valJObject(NULL), valJArray(NULL){
    ENSURE(properlyInitialized(), "JValue object hasn't been initialized properly!");
}
JValue::JValue(std::string val): initCheck(this), valString(val), valJObject(NULL), valJArray(NULL){
    ENSURE(properlyInitialized(), "JValue object hasn't been initialized properly!");
}
JValue::JValue(JObject* val): initCheck(this), valJObject(val), valJArray(NULL){
    ENSURE(properlyInitialized(), "JValue object hasn't been initialized properly!");
}
JValue::JValue(JArray* val): initCheck(this), valJObject(NULL), valJArray(val){
    ENSURE(properlyInitialized(), "JValue object hasn't been initialized properly!");
}
JValue::JValue(unsigned int val): initCheck(this), valUnsignedint(val), valJObject(NULL), valJArray(NULL) {
    ENSURE(properlyInitialized(), "JValue object hasn't been initialized properly!");
}

int JValue::asInt() const{
    REQUIRE(properlyInitialized(), "JValue object hasn't been initialized properly!");
    return valInt;
}
unsigned int JValue::asUnsignedint() const {
    REQUIRE(properlyInitialized(), "JValue object hasn't been initialized properly!");
    return valUnsignedint;
}
float JValue::asFloat() const{
    REQUIRE(properlyInitialized(), "JValue object hasn't been initialized properly!");
    return valFloat;
}
double JValue::asDouble() const{
    REQUIRE(properlyInitialized(), "JValue object hasn't been initialized properly!");
    return valDouble;
}
char JValue::asChar() const{
    REQUIRE(properlyInitialized(), "JValue object hasn't been initialized properly!");
    return valChar;
}
bool JValue::asBool() const{
    REQUIRE(properlyInitialized(), "JValue object hasn't been initialized properly!");
    return valBool;
}
std::string JValue::asString(){
    REQUIRE(properlyInitialized(), "JValue object hasn't been initialized properly!");
    return valString;
}
JObject* JValue::asJObject(){
    REQUIRE(properlyInitialized(), "JValue object hasn't been initialized properly!");
    return valJObject;
}

JArray* JValue::asJArray(){
    REQUIRE(properlyInitialized(), "JValue object hasn't been initialized properly!");
    return valJArray;
}

bool JValue::properlyInitialized() const {
    return initCheck == this;
}

JValue::~JValue() {
    // TODO: decide whether or not to keep this
    delete valJArray;
    delete valJObject;
    valJArray = NULL;
    valJObject = NULL;
}
