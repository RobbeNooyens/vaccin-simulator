// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#ifndef VACCIMULATOR_VACCIN_H
#define VACCIMULATOR_VACCIN_H


#include <string>
#include <vector>

#define DEFAULT_VACCINE "Standaard"

class JObject;
class Hub;

class Vaccine {
public:
    // Constructor
    /**
     * Vaccine default constructor
     * * ENSURE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
     */
    Vaccine();
    /**
     * Vaccine parameter constructor
     * @param type: string; name of the vaccine
     * @param delivery: unsigned int; the amount of vaccines delivered per delivery to the hub
     * @param interval: unsigned int; the amount of days between two deliveries to the hub
     * @param transportation: unsigned int; the amount of vaccines that can be transported to the centers per chunck
     * @param renewing: unsigned int; days between the first and the second sting
     * @param temperature: double; temperature required to store this vaccine
     * * ENSURE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
     */
    Vaccine(std::string type, unsigned int delivery, unsigned int interval, unsigned int transportation, unsigned int renewing = 0, double temperature = 0);

    // Initialization
    /**
     * Checks if the current object was initialized properly
     * @return bool; true if the initCheck pointer points to the current instance
     */
    bool properlyInitialized() const;

    // IO Mapping
    /**
     * Loads data from a JSON object
     * @param json: JObject; JObject that holds the data to load valid into the object
     * * REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
     * * REQUIRE(json != NULL, "JObject can't be NULL!");
     * * REQUIRE(json->contains(VACCINE_TYPE), "Vaccine JSON should contain field 'type'");
     * * REQUIRE(json->contains(VACCINE_DELIVERY), "Vaccine JSON should contain field 'levering'");
     * * REQUIRE(json->contains(VACCINE_INTERVAL), "Vaccine JSON should contain field 'interval'");
     * * REQUIRE(json->contains(VACCINE_TRANSPORTATION), "Vaccine JSON should contain field 'transport'");
     * * ENSURE(getType() == json->getValue(VACCINE_TYPE)->asString(), "Vaccine type didn't load properly!");
     * * ENSURE(getDelivery() == json->getValue(VACCINE_DELIVERY)->asUnsignedint(), "Vaccine delivery didn't load properly!");
     * * ENSURE(getInterval() == json->getValue(VACCINE_INTERVAL)->asUnsignedint(), "Vaccine interval didn't load properly!");
     * * ENSURE(getTransportation() == json->getValue(VACCINE_TRANSPORTATION)->asUnsignedint(), "Vaccine transportation didn't load properly!");
     * * ENSURE(getRenewing() == json->getValue(VACCINE_RENEWING)->asUnsignedint(), "Vaccine renewing didn't load properly!");
     * * ENSURE(getTemperature() == json->getValue(VACCINE_TEMPERATURE)->asDouble(), "Vaccine temperature didn't load properly!");
     */
    void fromJSON(JObject* json);

    // Getters
    /**
     * @return string; type of the vaccine
     * * REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
     */
    std::string getType() const;
    /**
     * @return unsigned int; the amount of vaccines that will be delivered to the hub per interval
     * * REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
     */
    unsigned int getDelivery() const;
    /**
     * @return unsigned int; the amount of days between two deliveries
     * * REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
     */
    unsigned int getInterval() const;
    /**
     * @return unsigned int; the amount of vaccines that can be transported per chunck
     * * REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
     */
    unsigned int getTransportation() const;
    /**
     * @return unsigned int; the amount of days between de first and the second sting
     * * REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
     */
    unsigned int getRenewing() const;
    /**
     * @return double; the required temperature to store the vaccine valid
     * * REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
     */
    double getTemperature() const;

    // Smart distribution
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
    std::vector<unsigned int>& getDays();
    /**
     * defines comparison between Vaccine objects
     * @param s: the Vaccine to be compared to
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    bool operator<(const Vaccine& s) const;
    /**
     * sets the hub connected with this vaccin
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    void setHub(Hub* h);


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

    // Smart distribution
    Hub* hub;
    std::vector<unsigned int> days;
    unsigned int dynamicDelivery;
};


#endif //VACCIMULATOR_VACCIN_H
