// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#ifndef VACCIMULATOR_VACCIN_H
#define VACCIMULATOR_VACCIN_H


#include <string>

class JObject;

class Vaccin {
public:
    // Constructor
    Vaccin();

    // Initialization
    bool properlyInitialized() const;

    // IO Mapping
    void fromJSON(JObject* json);

    // Getters
    std::string getType() const;
    unsigned int getDelivery() const;
    unsigned int getInterval() const;
    unsigned int getTransportation() const;
    unsigned int getRenewing() const;
    double getTemperature() const;


private:
    // Initialization
    const Vaccin* initCheck;

    // Metadata
    std::string type;
    unsigned int delivery;
    unsigned int interval;
    unsigned int transportation;
    unsigned int renewing;
    double temperature;


};


#endif //VACCIMULATOR_VACCIN_H
