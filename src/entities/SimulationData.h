//
// Created by robbe on 23/05/2021.
//

#ifndef VACCIMULATOR_SIMULATIONDATA_H
#define VACCIMULATOR_SIMULATIONDATA_H


#include <map>
#include <string>

class Vaccine;

// TODO: documentation
class SimulationData {
public:
    SimulationData();

    bool properlyInitialized();

    void addDelivery(Vaccine* vaccine, unsigned int amount);
    void addVaccinatedInhabitants(unsigned int amount);

    void printStatistics(std::ostream &stream) const;

    void reset();


private:
    const SimulationData* initCheck;

    std::map<std::string, unsigned int> vaccinsDelivered;
    unsigned int vaccinated;

};


#endif //VACCIMULATOR_SIMULATIONDATA_H
