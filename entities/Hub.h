// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#ifndef VACCIN_SIMULATOR_HUB_H
#define VACCIN_SIMULATOR_HUB_H

#include <vector>
#include <istream>
#include <ostream>

#include "../json/JObject.h"

class TiXmlElement;
class VaccinationCenter;

typedef std::vector<VaccinationCenter*> VaccinationCenters;

class Hub {
public:
    // Initialization
    const Hub* initCheck;

    // Metadata
    unsigned int delivery;
    unsigned int interval;
    unsigned int transport; // Vaccines per load
    unsigned int vaccins;

    // Connected vaccinationcenters
    VaccinationCenters centers;

    // Constructor
    /**
     * ENSURE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
    Hub();
	~Hub();
    bool properlyInitialized() const;

    // Getters
    /**
     * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     * @return
     */
    unsigned int getVaccins() const;

    // IO
    /**
     * Loads a hub from a JObject
     * @param json: JSON object containing the data for the hub and centers
     * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
    void fromJSON(JObject* json);
    void fromTiXMLElement(TiXmlElement* element);
    /**
     * Exports Hub object member to the given stream
     * @param ostream:
     * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
    void toStream(std::ostream&) const;

    // Events
    /**
     * Simulates the transportation and vaccination for one day
     * @param day: daynumber relative to the startdate
     * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
    void simulateDay(unsigned int day);

private:
    // Simulation
    /**
     * Simulate vaccin distribution over the vaccinationcenters
     */
    void distributeVaccins();
    /**
     * Transport a specific amount of vaccinations to the specified vaccination center.
     * @param center: the center where the vaccins should be transported to
     * @param vaccinCount: the amount of vaccins to transport
     */
    static void transportVaccinsTo(VaccinationCenter* center, unsigned int vaccinCount);
};


#endif //VACCIN_SIMULATOR_HUB_H
