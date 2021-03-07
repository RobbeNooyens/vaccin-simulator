//
// Created by robbe on 25/02/2021.
//

#ifndef VACCIN_SIMULATOR_HUB_H
#define VACCIN_SIMULATOR_HUB_H

#include <vector>
#include <istream>
#include <ostream>

class VaccinationCenter;

typedef std::vector<VaccinationCenter*> VaccinationCenters;

class Hub {
public:
    Hub();

    // Getters
    unsigned int getVaccins() const;

    // IO Mapping
    void fromStream(std::istream&);
    void toStream(std::ostream&) const;

private:
    unsigned int delivery;
    unsigned int interval;
    unsigned int transport;
    unsigned int vaccins;
    VaccinationCenters centers;
};


#endif //VACCIN_SIMULATOR_HUB_H
