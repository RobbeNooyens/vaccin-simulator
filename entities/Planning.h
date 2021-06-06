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
    // Constructor
    /**
     * ENSURE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
    Planning();


    // Destructor
    /**
     * ENSURE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
    ~Planning();


    // Validation
    /**
     * Checks if the current object was initialized properly
     * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     * @return bool; true if the initCheck pointer points to the current instance
     */
    bool properlyInitialized();
    /**
     * Checks if k, z and i are a valid combination of integers
     * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     * @return bool; true if the triple is valid
     */
    bool is_valid(unsigned int day, int vaccine_idx, int center_idx, std::vector<Vaccine*>& vaccins, std::vector<VaccinationCenter*>& centers, std::vector<Hub*>& hubs, unsigned int cycles);


    // Initialization
    /**
     * generates the plan for the simulation
     * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
    void generatePlanning(std::vector<Hub*>& hubs, std::vector<VaccinationCenter*>& centers, std::vector<Vaccine*>& vaccins, unsigned int cycles);


    // Getters
    /**
     * @return std::vector<std::vector<std::pair<int,std::map<Vaccine*, int>>>>; the plan calender for the simulation
     * REQUIRE(properlyInitialized(), "Hub object hasn't been initialized properly!");
     */
    std::vector<std::vector<std::pair<int,std::map<Vaccine*, int>>>> getPlanned() const;

    std::map<Vaccine*, unsigned int>& getDistribution(unsigned int day, VaccinationCenter* center);

private:
    // Initialization
    Planning* initCheck;

    // metadata
    std::vector<std::vector<std::pair<int,std::map<Vaccine*, int>>>> planned;

    // map from center name to index in "centers" vector
    map<Center*, int> center2idx;

};


#endif //VACCIMULATOR_PLANNING_H
