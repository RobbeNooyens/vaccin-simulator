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
    unsigned int transport;
    unsigned int vaccins;

    // Connected vaccinationcenters
    VaccinationCenters centers;

    // Constructor
    Hub();
	~Hub();
    bool properlyInitialized() const;

    // Getters
    unsigned int getVaccins() const;

    // IO
    void fromTiXMLElement(TiXmlElement* element);
    void toStream(std::ostream&) const;

    // Events
    void simulateDay(unsigned int day);

private:
    // Simulation
    void distributeVaccins();
    static void transportVaccinsTo(VaccinationCenter* center, unsigned int vaccinCount);
};


#endif //VACCIN_SIMULATOR_HUB_H
