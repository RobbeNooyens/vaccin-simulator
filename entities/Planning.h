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

    void generatePlanning(std::vector<Hub*>& hubs, std::vector<VaccinationCenter*>& centers, std::vector<Vaccine*>& vaccins, unsigned int cycles);

    bool is_valid(unsigned int k, int z, int i, std::vector<Vaccine*>& vaccins, std::vector<VaccinationCenter*>& centers, std::vector<Hub*>& hubs, unsigned int cycles);

    std::vector<std::vector<std::pair<int,std::map<Vaccine*, int>>>> planned;
private:
    Planning* initCheck;
};


#endif //VACCIMULATOR_PLANNING_H
