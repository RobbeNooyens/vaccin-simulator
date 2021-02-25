//
// Created by robbe on 25/02/2021.
//

#ifndef VACCIN_SIMULATOR_SIMULATOR_H
#define VACCIN_SIMULATOR_SIMULATOR_H

#include "domain/Hub.h"

class Simulator {
public:
    Simulator();

    // Run
    void run(const unsigned int cycles);
//    void reset();

private:
    Hub hub;
    unsigned int daycount;

    void cycle();

};


#endif //VACCIN_SIMULATOR_SIMULATOR_H
