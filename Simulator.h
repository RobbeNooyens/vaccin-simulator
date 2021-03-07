//
// Created by robbe on 25/02/2021.
//

#ifndef VACCIN_SIMULATOR_SIMULATOR_H
#define VACCIN_SIMULATOR_SIMULATOR_H

#include "entities/Hub.h"
#include <string>

class Simulator {
public:
    Simulator();

    // IO
    void importSimulation(const std::string& fileName);
    void exportSimulation(const std::string& fileName);

    // Run
    void run(unsigned int cycles);
//    void reset();

private:
    Hub hub;
    unsigned int daycount;

    void cycle();

};


#endif //VACCIN_SIMULATOR_SIMULATOR_H
