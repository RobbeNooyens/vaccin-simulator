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
    std::string name;
    std::string address;
    unsigned int vaccins;
    unsigned int inhabitants;
    unsigned int vaccinated;
    unsigned int capacity;

    // Constructor
    VaccinationCenter()=default;
    VaccinationCenter(std::string name, std::string address, unsigned int inhabitants, unsigned int capacity);
    bool properlyInitialized() const;

    // Simulation
    void transportationArrived(unsigned int vaccinCount);
    void vaccinateInhabitants();

    // Getters
    std::string getName() const;
    std::string getAddress() const;
    unsigned int getInhabitants() const;
    unsigned int getCapacity() const;
    unsigned int getVaccins() const;
    unsigned int getVaccinationsDone() const;
    unsigned int getVaccinationsLeft() const;
	void setName(std::string name);
	void setAddress(std::string address);
	void setInhabitants(unsigned int a);
	void setCapacity(unsigned int a);

    // IO
    void fromTiXMLElement(TiXmlElement*);
    void toStream(std::ostream&) const;
};


#endif //VACCIN_SIMULATOR_VACCINATIONCENTER_H
