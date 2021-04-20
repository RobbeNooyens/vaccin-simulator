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

class Vaccine {
public:
    // Constructor
    Vaccine();
    Vaccine(std::string type, unsigned int delivery, unsigned int interval, unsigned int transportation, unsigned int renewing = 0, double temperature = 0);

    // Initialization
    /**
     * Checks if the current object was initialized properly
     * @return bool; true if the initCheck pointer points to the current instance
     */
    bool properlyInitialized() const;

    // IO Mapping
    void fromJSON(JObject* json);
    JObject* toJSON() const;

    // Getters
    std::string getType() const;
    unsigned int getDelivery() const;
    unsigned int getInterval() const;
    unsigned int getTransportation() const;
    unsigned int getRenewing() const;
    double getTemperature() const;


private:
    // Initialization
    const Vaccine* initCheck;

    // Metadata
    std::string type;
    unsigned int delivery;
    unsigned int interval;
    unsigned int transportation;
    unsigned int renewing;
    double temperature;


};


#endif //VACCIMULATOR_VACCIN_H
