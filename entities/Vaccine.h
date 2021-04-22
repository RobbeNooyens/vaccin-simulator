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

    // Initialization
    /**
     * Checks if the current object was initialized properly
     * @return bool; true if the initCheck pointer points to the current instance
     */
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

    bool operator<(const Vaccine& s) const;

    Hub* getHub() const;

    void setHub(Hub* h);

    vector<unsigned int>& getDays() const;

private:
    vector<unsigned int> days;
    // Initialization
    const Vaccine* initCheck;

    // Metadata
    Hub* hub;
    std::string type;
    unsigned int delivery;
    unsigned int interval;
    unsigned int transportation;
    unsigned int renewing;
    double temperature;


};


#endif //VACCIMULATOR_VACCIN_H
