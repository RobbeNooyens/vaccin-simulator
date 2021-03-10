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

class VaccinationCenter {
public:
    // Initialization
    const VaccinationCenter* initCheck;

    // Metadata
    const std::string name;
    const std::string address;
    unsigned int vaccins;
    unsigned int inhabitants;
    unsigned int vaccinated;
    unsigned int capacity;

    // Constructor
    VaccinationCenter(std::string name, std::string address, unsigned int inhabitants, unsigned int capacity);
    bool properlyInitialized() const;

    // Simulation
    void transportArrived(unsigned int vaccinCount);
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
    void fromTiXMLElement(TiXmlElement*);
    void toStream(std::ostream&) const;
};


#endif //VACCIN_SIMULATOR_VACCINATIONCENTER_H
