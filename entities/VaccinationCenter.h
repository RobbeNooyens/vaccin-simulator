// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#ifndef VACCIN_SIMULATOR_VACCINATIONCENTER_H
#define VACCIN_SIMULATOR_VACCINATIONCENTER_H

#include <string>

class TiXmlElement;
class JObject;

class VaccinationCenter {
public:
    // Constructor
    /**
     * Default constructor
     * ENSURE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    VaccinationCenter();
    /**
     * VaccionationCenter parameter constructor
     * @param name: center's name
     * @param address: the address where the center is located
     * @param inhabitants: the amount of inhabitants that should be vaccinated in this center
     * @param capacity: the max amount of people that can be vaccinated here daily
     * ENSURE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    VaccinationCenter(std::string name, std::string address, unsigned int inhabitants, unsigned int capacity);
    bool properlyInitialized() const;

    // Simulation
    void transportationArrived(unsigned int vaccinCount);
    /**
     * Simulate the situation where the capacity amount of people are being vaccinated
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    void vaccinateInhabitants();

    // Getters
    std::string getName() const;
    std::string getAddress() const;
    unsigned int getInhabitants() const;
    unsigned int getCapacity() const;
    unsigned int getVaccins() const;
    unsigned int getVaccinationsDone() const;
    unsigned int getVaccinationsLeft() const;

    // IO
    /**
     * Load a VaccinationCenter from a Json object
     * @param json: the Json object representing a VaccinationCenter
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     * REQUIRE(json != NULL, "Json can't be NULL!");
     */
    void fromJSON(JObject* json);
    /**
     * Sends the center represented as a string to the given stream
     * @param stream: The stream that should be used
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    void toStream(std::ostream& stream) const;

private:
    // Initialization
    const VaccinationCenter* initCheck;

    // Metadata
    std::string name;
    std::string address;
    unsigned int vaccins;
    unsigned int inhabitants;
    unsigned int vaccinated;
    unsigned int capacity;
};


#endif //VACCIN_SIMULATOR_VACCINATIONCENTER_H
