// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include "JValue.h"
#include "JObject.h"
#include "JArray.h"

JValue::JValue(): initCheck(this) {}
JValue::JValue(int val): initCheck(this), valInt(val){}
JValue::JValue(float val): initCheck(this), valFloat(val){}
JValue::JValue(double val): initCheck(this), valDouble(val){}
JValue::JValue(char val): initCheck(this), valChar(val){}
JValue::JValue(bool val): initCheck(this), valBool(val){}
JValue::JValue(std::string val): initCheck(this), valString(val){}
JValue::JValue(JObject* val): initCheck(this), valJObject(val){}
JValue::JValue(JArray* val): initCheck(this), valJArray(val){}

JValue::JValue(unsigned int val): initCheck(this), valUnsignedint(val) {}
int JValue::asInt() const{ return valInt; }
unsigned int JValue::asUnsignedint() const { return valUnsignedint;}
float JValue::asFloat() const{ return valFloat;}
double JValue::asDouble() const{ return valDouble; }
char JValue::asChar() const{ return valChar; }
bool JValue::asBool() const{ return valBool; }
std::string JValue::asString(){ return valString; }
JObject* JValue::asJObject(){ return valJObject; }

JArray* JValue::asJArray(){ return valJArray; }

bool JValue::properlyInitialized() { return initCheck == this; }
