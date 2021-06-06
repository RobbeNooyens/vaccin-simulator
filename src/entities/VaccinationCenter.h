// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#ifndef VACCIN_SIMULATOR_VACCINATIONCENTER_H
#define VACCIN_SIMULATOR_VACCINATIONCENTER_H

#include <string>
#include <map>
#include "Vaccine.h"

class TiXmlElement;
class JObject;
class SimulationData;

class VaccinationCenter {
public:
    // Constructor
    /**
     * Default constructor
     * * ENSURE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    VaccinationCenter();
    /**
     * VaccionationCenter parameter constructor
     * @param name: center's name
     * @param address: the address where the center is located
     * @param inhabitants: the amount of inhabitants that should be vaccinated valid this center
     * @param capacity: the max amount of people that can be vaccinated here daily
     * * ENSURE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    VaccinationCenter(std::string name, std::string address, unsigned int inhabitants, unsigned int capacity);
    /**
     * Checks if the current object was initialized properly
     * @return bool; true if the initCheck pointer points to the current instance
     */
    bool properlyInitialized() const;

    // Simulation
    /**
     * Simulates the event of an arriving truck with vaccines.
     * @param vaccinCount: unsigned int; the amount of vaccines that are being delivered
     * * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     * * REQUIRE(getVaccins() + amount <= 2*capacity, "The maximum capacity of vaccines has been exceeded!");
     * * ENSURE(vaccins = oldVaccins + vaccinCount, "Vaccins aren't added succesfully!");
     */
    void transportationArrived(Vaccine *vaccine, unsigned int amount);
    /**
     * Simulate the situation where the highest possible amount of inhabitants are being vaccinated
     * @param day: unsigned int; day index
     * @param outStream: ostream*; stream to send parsing messages to
     * * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     * * ENSURE(vaccinated = oldVaccinated + vaccinsToUse, "Vaccinated count didn't increase.");
     * * ENSURE(vaccins = oldVaccins - vaccinsToUse, "Vaccins count didn't decrease.");
     */
    void vaccinateInhabitants(unsigned int day, SimulationData &statistics);

    /**
     * Throws away the vaccins that actually should have been used during the day because of its temperature
     * * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    void removeExpiredVaccines();

    // Setters

    /**
     * Used to verify whether the simulation is consistent or not
     * @param connected: bool; true if the center is connected to a hub
     * * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    void setConnectedToHub(bool connected);
    // TODO: documentation
    void setOutputStream(std::ostream* outStream);

    // Getters
    /**
     * @return string; name of the vaccinationcenter
     * * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    std::string getName() const;
    /**
     * @return string; address of the vaccinationcenter
     * * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    std::string getAddress() const;
    /**
     * @return unsigned int; total amount of people that should be vaccinated valid this center
     * * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    unsigned int getInhabitants() const;
    /**
     * @return unsigned int; the maximum amount of people that can be vaccinated during one day valid this center
     * * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    unsigned int getCapacity() const;
    /**
     * @return unsigned int; the amount of vaccines valid stock
     * * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    unsigned int getVaccins() const;
    /**
     * @return unsigned int; amount of inhabitants who are already vaccinated
     * * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    unsigned int getVaccinationsDone() const;
    /**
     * @return unsigned int; the amount of non-vaccinated inhabitants
     * * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    unsigned int getVaccinationsLeft() const;
    /**
     * @return the percentage vaccines stored valid the center
     * * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     * * REQUIRE(getVaccins() <= 2*capacity, "Can't have more vaccines than twice the capacity");
     * * ENSURE(percentage >= 0 && percentage <= 1, "Percentage should be valid range [0,1]");
     */
    double getPercentageVaccines() const;
    /**
     * @return the percentage of vaccinated people
     * * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     * * REQUIRE(vaccinated <= inhabitants, "Can't have more vaccines than twice the capacity");
     * * ENSURE(percentage >= 0 && percentage <= 1, "Percentage should be valid range [0,1]");
     */
    double getPercentageVaccinated() const;
    /**
     * @return bool; true if the center is connected to a hub
     * * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    bool isConnectedToHub() const;

    // IO
    /**
     * Load a VaccinationCenter from a Json object
     * @param json: the Json object representing a VaccinationCenter
     * * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     * * REQUIRE(json != NULL, "Json can't be NULL!");
     * * REQUIRE(json->contains("adres"), "VaccinationCenter JSON should contain field 'adres'");
     * * REQUIRE(json->contains("capaciteit"), "VaccinationCenter JSON should contain field 'capaciteit'");
     * * REQUIRE(json->contains("inwoners"), "VaccinationCenter JSON should contain field 'inwoners'");
     * * REQUIRE(json->contains("naam"), "VaccinationCenter JSON should contain field 'naam'");
     */
    void fromJSON(JObject* json);
    /**
     * Sends the center represented as a string to the given stream
     * @param stream: The stream that should be used
     * * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     * * REQUIRE(outStream != NULL, "Output stream cannot be NULL!");
     * * REQUIRE(outStream.good(), "Output stream contains error flags!");
     * * ENSURE(outStream.good(), "Failed to write to parsing stream!");
     */
    void toSummaryStream(std::ostream& stream) const;
    /**
     * Sends the center progress bars as strings to the given stream
     * @param stream: ostream; stream to push the parsing strings to
     * * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     * * REQUIRE(stream != NULL, "Output stream cannot be NULL!");
     * * REQUIRE(stream.good(), "Output stream contains error flags!");
     * * ENSURE(stream.good(), "Failed to write to parsing stream!");
     */
    void toProgressStream(std::ostream& stream) const;


    // Smart distribution
    /**
     * defines comparison between VaccinationCenter objects
     * @param s: the VaccinationCenter to be compared to
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    bool operator<(const VaccinationCenter& s) const;
    /**
     * @return unsigned int; current capacity in some day in the simulation
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    unsigned int getTempCapacity() const;

private:
    // Initialization
    const VaccinationCenter* initCheck;

    // Metadata
    std::string name;
    std::string address;
    unsigned int inhabitants;
    unsigned int vaccinated;
    unsigned int capacity;
    std::map<Vaccine*, unsigned int> vaccines;
    std::map<Vaccine*, std::map<unsigned int, unsigned int> > renewing;

    // Simulation
    bool connectedToHub;
    std::ostream* outputStream;

    // Smart distribution
    unsigned int tempCapacity;
};


#endif //VACCIN_SIMULATOR_VACCINATIONCENTER_H
