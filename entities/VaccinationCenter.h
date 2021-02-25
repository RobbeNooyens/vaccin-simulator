//
// Created by robbe on 25/02/2021.
//

#ifndef VACCIN_SIMULATOR_VACCINATIONCENTER_H
#define VACCIN_SIMULATOR_VACCINATIONCENTER_H

#include <string>


class VaccinationCenter {
public:
    VaccinationCenter(std::string name, std::string address, unsigned int inhabitants, unsigned int capacity);

    // Getters
    std::string getName() const;
    std::string getAddress() const;
    unsigned int getInhabitants() const;
    unsigned int getCapacity() const;
    unsigned int getVaccins() const;
    unsigned int getVaccinationsDone() const;
    unsigned int getVaccinationsLeft() const;

    // IO Mapping
    void toStream(std::ostream) const;

private:
    const std::string name;
    const std::string address;
    unsigned int vaccins;
    unsigned int inhabitants;
    unsigned int vaccinated;
    unsigned int capacity;

};


#endif //VACCIN_SIMULATOR_VACCINATIONCENTER_H
