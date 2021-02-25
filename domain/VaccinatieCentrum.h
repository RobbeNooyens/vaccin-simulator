//
// Created by robbe on 25/02/2021.
//

#ifndef VACCIN_SIMULATOR_VACCINATIECENTRUM_H
#define VACCIN_SIMULATOR_VACCINATIECENTRUM_H

#include <string>

class VaccinatieCentrum {
public:
    VaccinatieCentrum(std::string naam, std::string adres, unsigned int inwoners, unsigned int capaciteit);

    // Getters
    std::string getNaam() const;
    std::string getAdres() const;
    unsigned int getInwoners() const;
    unsigned int getCapaciteit() const;

private:
    const std::string naam;
    const std::string adres;
    unsigned int inwoners;
    unsigned int capaciteit;

};


#endif //VACCIN_SIMULATOR_VACCINATIECENTRUM_H
