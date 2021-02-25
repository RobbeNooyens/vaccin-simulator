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
    void import(std::string file);
    void export(std::string file);

    // Run
    void run(const unsigned int cycles);
//    void reset();

private:
    Hub hub;
    unsigned int daycount;

    void cycle();

};


#endif //VACCIN_SIMULATOR_SIMULATOR_H
