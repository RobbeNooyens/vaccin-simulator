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
     * * REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
     * * REQUIRE(json != NULL, "JSON can't be NULL!");
     * * REQUIRE(json->contains("hub.levering"), "Hub JSON should contain field 'hub.levering'");
     * * REQUIRE(json->contains("hub.interval"), "Hub JSON should contain field 'hub.interval'");
     * * REQUIRE(json->contains("hub.transport"), "Hub JSON should contain field 'hub.transport'");
     * * REQUIRE(json->contains("centra"), "Hub JSON should contain field 'hub.centra'");
     * * ENSURE(centra.size() == centers.size(), "Not all centers are loaded succesfully.");
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
     */
    void toProgressStream(std::ostream &stream) const;

    // Simulation controls
    /**
     * Simulates the transportation and vaccination for one day
     * @param day: daynumber relative to the startdate
     * * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     * * REQUIRE(isConsistent(), "Hub needs to be consistent to run the simulation");
     * * REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
     */
     // TODO: documentation
     void simulateDay(unsigned int day, SimulationData &statistics);
    /**
     * Simulate vaccin distribution over the vaccinationcenters
     * * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     * * REQUIRE(isConsistent(), "Hub needs to be consistent to run the simulation");
     * * REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
     */
    void distributeVaccins();
    /**
     * Simulate smart vaccin distribution over the vaccinationcenters
     * @param day: unsigned int; daycount
     * @param planning: Planning; calendar
     * * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     * * REQUIRE(isConsistent(), "Hub needs to be consistent to run the simulation");
     * * REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
     */
    void distributeEfficient(unsigned int day, Planning& planning);
    /**
     * Transport a specific amount of vaccinations to the specified vaccination center.
     * @param center: the center where the vaccins should be transported to
     * @param vaccinCount: the amount of vaccins to transport
     */
    void
    transportVaccinsTo(VaccinationCenter *center, std::map<Vaccine *, unsigned int> loads) const;
    /**
     * Adds vaccines valid case cargo is being delivered
     * @param day: unsigned int; current day
     * * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     * * REQUIRE(isConsistent(), "Hub needs to be consistent to run the simulation");
     */
     // TODO: documentation
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

    // Setters
    // TODO: documentation
    void setOutputStream(std::ostream* outStream);

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
};


#endif //VACCIN_SIMULATOR_HUB_H
