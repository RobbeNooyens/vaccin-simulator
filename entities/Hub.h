// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

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

typedef std::vector<VaccinationCenter*> VaccinationCenters;

class Hub {
public:
    // Constructor
    /**
     * ENSURE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
    Hub();
    /**
     * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
	~Hub();
    /**
     * Checks if the current object was initialized properly
     * @return bool; true if the initCheck pointer points to the current instance
     */
    bool properlyInitialized() const;

    // Getters
    /**
     * @return the amount of vaccins there are currently available in the hub
     * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
    std::vector<Vaccine*>& getVaccins() const;

    // IO
    /**
     * Loads a hub from a JObject
     * @param json: JSON object containing the data for the hub and centers
     * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     * REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
     * REQUIRE(json != NULL, "JSON can't be NULL!");
     * REQUIRE(json->contains("hub.levering"), "Hub JSON should contain field 'hub.levering'");
     * REQUIRE(json->contains("hub.interval"), "Hub JSON should contain field 'hub.interval'");
     * REQUIRE(json->contains("hub.transport"), "Hub JSON should contain field 'hub.transport'");
     * REQUIRE(json->contains("centra"), "Hub JSON should contain field 'hub.centra'");
     * ENSURE(centra.size() == centers.size(), "Not all centers are loaded succesfully.");
     */
    void fromJSON(JObject* json);
    /**
     * Exports Hub object member to the given stream
     * @param ostream:
     * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     * REQUIRE(outStream != NULL, "Output stream cannot be NULL!");
     * REQUIRE(outStream.good(), "Output stream contains error flags!");
     * REQUIRE(!containsInvalidCenter(), "Hub contains an invalid center!");
     * ENSURE(outStream.good(), "Failed to write to output stream!");
     */
    void toSummaryStream(std::ostream&) const;
    void toProgressStream(std::ostream&) const;

    // Events
    /**
     * Simulates the transportation and vaccination for one day
     * @param day: daynumber relative to the startdate
     * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
    // void simulateDay(unsigned int day);

    // Validations
    /**
     * Checks if any of the saved centers is invalid
     * @return bool: true if this hub contains an invalid center
     * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     * REQUIRE(&centers != NULL, "Centers can't be NULL!");
     */
    // bool containsInvalidCenter() const;

    bool is_connected(const std::string name) const;

private:
    unsigned int connections;
    std::vector<Vaccine*> vaccins;
    map<std::string, bool> centers;
    // Initialization
    const Hub* initCheck;

    // Metadata
    //unsigned int delivery;
    //unsigned int interval;
    //unsigned int transport; // Vaccines per load
    //unsigned int vaccinsCount;

    // Connected vaccinationcenters
    //VaccinationCenters centers;

    // Amount of vaccines per type
    //std::map<Vaccine*, int> vaccinCount;

    // Simulation
    /**
     * Simulate vaccin distribution over the vaccinationcenters
     * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
    // void distributeVaccins(std::ostream& outStream = std::cout);
    /**
     * Transport a specific amount of vaccinations to the specified vaccination center.
     * @param center: the center where the vaccins should be transported to
     * @param vaccinCount: the amount of vaccins to transport
     */
    // static void transportVaccinsTo(VaccinationCenter* center, unsigned int vaccinCount);
};


#endif //VACCIN_SIMULATOR_HUB_H
