// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include "JValue.h"
#include "JObject.h"
#include "JArray.h"

JValue::JValue(int val): valInt(val){}
JValue::JValue(float val): valFloat(val){}
JValue::JValue(double val): valDouble(val){}
JValue::JValue(char val): valChar(val){}
JValue::JValue(bool val): valBool(val){}
JValue::JValue(std::string val): valString(val){}
JValue::JValue(JObject* val): valJObject(val){}
JValue::JValue(JArray* val): valJArray(val){}
JValue::JValue(unsigned int val): valUnsignedint(val) {}

int JValue::asInt() const{ return valInt; }
unsigned int JValue::asUnsignedint() const { return valUnsignedint;}
float JValue::asFloat() const{ return valFloat;}
double JValue::asDouble() const{ return valDouble; }
char JValue::asChar() const{ return valChar; }
bool JValue::asBool() const{ return valBool; }
std::string JValue::asString(){ return valString; }
JObject* JValue::asJObject(){ return valJObject; }
JArray* JValue::asJArray(){ return valJArray; }