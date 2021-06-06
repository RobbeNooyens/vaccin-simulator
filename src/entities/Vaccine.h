// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#ifndef VACCIMULATOR_VACCIN_H
#define VACCIMULATOR_VACCIN_H


#include <string>

#define DEFAULT_VACCINE "Standaard"

class JObject;

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
     */
    void fromJSON(JObject* json);
    /**
     * Creates a JObject that holds the current data
     * @return JObject; JObject with the vaccine data
     * * REQUIRE(properlyInitialized(), "Vaccine object wasn't initialized properly!");
     */
    JObject* toJSON() const;

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
