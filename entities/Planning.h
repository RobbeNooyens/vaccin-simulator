// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#ifndef VACCIMULATOR_PLANNING_H
#define VACCIMULATOR_PLANNING_H

// TODO: documentation + tests


#include <map>

class Vaccine;
class VaccinationCenter;
class Hub;

class Planning {
public:
    Planning();
    ~Planning();
    bool properlyInitialized();

    void generatePlanning(unsigned int days, std::vector<Hub*> hubs, std::vector<VaccinationCenter*> centers, unsigned int startFrom = 0);

    std::map<Vaccine*, unsigned int> getDistribution(unsigned int day, VaccinationCenter* center);

private:
    Planning* initCheck;

};


#endif //VACCIMULATOR_PLANNING_H
