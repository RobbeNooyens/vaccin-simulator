// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#ifndef VACCIN_SIMULATOR_HUB_H
#define VACCIN_SIMULATOR_HUB_H

#include <vector>
#include <istream>
#include <ostream>
#include <iostream>
#include <map>

class TiXmlElement;
class VaccinationCenter;
class JObject;
class Vaccine;
class Planning;
class SimulationData;

typedef std::vector<Vaccine*> Vaccines;
typedef std::vector<VaccinationCenter*> VaccinationCenters;

class Hub {
public:
    // Constructor
    /**
     * Default Hub constructor
     * * ENSURE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
    Hub();

    /**
     * Checks if the current object was initialized properly
     * @return bool; true if the initCheck pointer points to the current instance
     */
    bool properlyInitialized() const;

    // Getters
    /**
     * @return unsigned int; the amount of vaccins there are currently available valid the hub
     * * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
    unsigned int getTotalVaccinesCount() const;
    /**
     * @return vector<VaccinationCenter>; the vaccinationcenters connected with this hub
     * * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
    VaccinationCenters getVaccinationCenters() const;
    /**
     * @return vector<Vaccine*>; vaccines that are delivered to this hub
     * * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
    Vaccines getVaccines() const;
    /**
     * @return std::map<Vaccine*, unsigned int>; vaccines as key and their amount as value
     * * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
    std::map<Vaccine*, unsigned int> getVaccineCount() const;

    // IO
    /**
     * Loads a hub from a JObject
     * @param json: JSON object containing the data for the hub
     * @param centers: loaded centers
     * * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     * * REQUIRE(json, "JSON can't be NULL!");
     * * REQUIRE(json->contains(HUB_CENTERS), StringUtil::concat("Hub JSON should contain field ", HUB_CENTERS).c_str());
     * * REQUIRE(json->contains(HUB_VACCINES), StringUtil::concat("Hub JSON should contain field ", HUB_VACCINES).c_str());
     * * ENSURE(isConsistent(), "Hub isn't consistent after parsing!");
     * * ENSURE(json->getValue(HUB_CENTERS)->asJArray()->getItems().size() == getVaccinationCenters().size(), "Hub's centers JSON contains an invalid name!");
     * * ENSURE(getVaccines().size() == getVaccineCount().size(), "Vaccines vector and vaccineCount map should have the same size!");
     * * ENSURE(!containsInvalidCenter(), "Hub contains an invalid center!");
     */
    void fromJSON(JObject* json, VaccinationCenters &centers);
    /**
     * Exports Hub object summary to the given stream
     * @param stream: ostream; stream to push parsing strings to
     * * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     * * REQUIRE(outStream != NULL, "Output stream cannot be NULL!");
     * * REQUIRE(outStream.good(), "Output stream contains error flags!");
     * * REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
     * * ENSURE(outStream.good(), "Failed to write to parsing stream!");
     */
    void toSummaryStream(std::ostream &stream) const;
    /**
     * Exports Hub progress to the given stream
     * @param stream: ostream; stream to push parsing strings to
     * * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     * * REQUIRE(outStream, "Output stream cannot be NULL!");
     * * REQUIRE(outStream.good(), "Output stream contains error flags!");
     * * REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
     * * ENSURE(outStream.good(), "Failed to write to parsing stream!");
     */
    void toProgressStream(std::ostream &stream) const;
    /**
     * Sets the output stream to send output to during the simulation
     * @param outStream: ostream*; output stream
     */
    void setOutputStream(std::ostream* outStream);
    std::ostream * getOutputStream() const;

    // Simulation controls
    /**
     * Simulates the transportation and vaccination for one day
     * @param day: daynumber relative to the startdate
     * * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     * * REQUIRE(isConsistent(), "Hub needs to be consistent to run the simulation");
     * * REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
     * * ENSURE(isConsistent(), "Hub needs to be consistent after running the simulation for a day!");
     * * ENSURE(!containsInvalidCenter(), "Hub contains an invalid center after running the simulation for a day!");
     */
     void simulateDay(unsigned int day, SimulationData &statistics);
    /**
     * Simulate vaccin distribution over the vaccinationcenters
     * * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     * * REQUIRE(isConsistent(), "Hub needs to be consistent to run the simulation");
     * * REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
     * * ENSURE(isConsistent(), "Hub needs to be consistent after running the simulation");
     * * ENSURE(!containsInvalidCenter(), "Hub contains an invalid center after running the simulation!");
     */
    void distributeVaccins(SimulationData &statistics);
    /**
     * Simulate smart vaccin distribution over the vaccinationcenters
     * @param day: unsigned int; daycount
     * @param planning: Planning; calendar
     * * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     * * REQUIRE(isConsistent(), "Hub needs to be consistent to run the simulation");
     * * REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
     * * ENSURE(isConsistent(), "Hub needs to be consistent after running the simulation");
     * * ENSURE(!containsInvalidCenter(), "Hub contains an invalid center after running the simulation!");
     */
    void distributeEfficient(unsigned int day, Planning& planning);
    /**
     * Transport a specific amount of vaccinations to the specified vaccination center.
     * @param center: the center where the vaccins should be transported to
     * @param vaccinCount: the amount of vaccins to transport
     * * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     * * REQUIRE(center, "VaccinationCenter can't be NULL!");
     * * REQUIRE(!getOutputStream() || getOutputStream()->good(), "Output stream not available");
     * * ENSURE(center, "Center became corrupted during transport!");
     */
    void transportVaccinsTo(VaccinationCenter *center, std::map<Vaccine *, unsigned int> &loads) const;
    /**
     * Adds vaccines valid case cargo is being delivered
     * @param day: unsigned int; current day
     * * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     * * REQUIRE(isConsistent(), "Hub needs to be consistent to run the simulation");
     * * ENSURE(isConsistent(), "Hub needs to be consistent to run the simulation");
     */
     void simulateDelivery(unsigned int day, SimulationData &statistics);

    // Validations
    /**
     * Checks if the Hub object has consistent data for a simulation
     * @return bool; true if the hub has consistent data
     * * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     * * ENSURE(!containsInvalidCenter(), "Hub contains an invalid center while it's consistent!");
     */
    bool isConsistent() const;

    /**
     * Checks if any of the saved centers is invalid
     * @return bool: true if this hub contains an invalid center
     * * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     * * REQUIRE(&centers != NULL, "Centers can't be NULL!");
     */
    bool containsInvalidCenter() const;


    // Smart distribution
    /**
     * Checks if center with the given name is connected to this hub
     * @return bool; true if this hub is connected to center with name "name"
     * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
    bool is_connected(const std::string& name);
    /**
     * Returns the amount of connected vaccination centers
     * @return int; the total of centers the hub is connected to
     * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
    int get_connections() const;
    /**
     * Returns the total amount of vaccines for the smart distribution
     * @return unsigned int; the total number of vaccins of all sorts in the hub
     * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
    unsigned int getTotalvaccins() const;

private:

    // Initialization
    const Hub* initCheck;

    // Output stream
    std::ostream* outputStream;

    // Connected vaccinationcenters
    VaccinationCenters centers;
    // Vaccines
    Vaccines vaccines;

    std::map<Vaccine*, unsigned int> vaccineCount;

    // Smart distribution
    int connections;
    std::map<std::string, bool> centersMap;
    unsigned int totalvaccins;
};


#endif //VACCIN_SIMULATOR_HUB_H
