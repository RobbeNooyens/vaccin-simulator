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
    /**
     * constructor
     * ENSURE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    Vaccine();


    // Initialization
    /**
     * Checks if the current object was initialized properly
     * @return bool; true if the initCheck pointer points to the current instance
     */
    bool properlyInitialized() const;


    // IO Mapping
    /**
     * parses and loads json objects into Vaccine objects
     * ENSURE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    void fromJSON(JObject* json);


    // Getters
    /**
     * @return string; the name of the Vaccine
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    std::string getType() const;
    /**
     * @return unsigned int; the delivery amount of the vaccins
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    unsigned int getDelivery() const;
    /**
     * @return unsigned int; total duration to get delivery refil for vaccins
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    unsigned int getInterval() const;
    /**
     * @return unsigned int; amount of vaccins that can get transported within 1 load
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    unsigned int getTransportation() const;
    /**
     * @return unsigned int; the amount of days where the vaccin needs to get reinjected
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    unsigned int getRenewing() const;
    /**
     * @return unsigned int; temperature of a vaccin
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    double getTemperature() const;
    /**
     * @return unsigned int; dynamic, with simulation changing delivery of vaccins, total number of vaccins currently for this particular vaccin
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    unsigned int getDynamicDelivery() const;
    /**
     * @return Hub*; dynamic, the particular hub connected with this particular vaccin object
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    Hub* getHub() const;
    /**
     * @return vector<unsigned int>&; vector with information over total collected vaccins over time
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    vector<unsigned int>& getDays() const;
    /**
     * defines comparison between Vaccine objects
     * @param s: the Vaccine to be compared to
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    bool operator<(const Vaccine& s) const;


    //SETTERS
    /**
     * sets the hub connected with this vaccin
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    void setHub(Hub* h);


private:
    // Initialization
    const Vaccine* initCheck;

    // Metadata
    Hub* hub;
    std::string type;
    unsigned int delivery;
    unsigned int dynamicDelivery;
    unsigned int interval;
    unsigned int transportation;
    unsigned int renewing;
    double temperature;
    vector<unsigned int> days;
};


#endif //VACCIMULATOR_VACCIN_H
