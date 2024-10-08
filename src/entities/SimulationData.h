// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#ifndef VACCIMULATOR_SIMULATIONDATA_H
#define VACCIMULATOR_SIMULATIONDATA_H


#include <map>
#include <string>
#include <vector>

class Vaccine;
class Hub;
class VaccinationCenter;

class SimulationData {
public:
    /**
     * Default constructor
     * * ENSURE(properlyInitialized(), "SimulationData wasn't initialized properly!");
     */
    SimulationData();

    /**
     * Checks if the current object was initialized properly
     * @return bool; true if the initCheck pointer points to the current instance
     */
    bool properlyInitialized() const;

    /**
     * Adds a delivery to the statistics
     * @param vaccine: Vaccine*; vaccine that was delivered
     * @param amount: unsigned int; amount of vaccines to add for this vaccine
     * * REQUIRE(properlyInitialized(), "SimulationData wasn't initialized properly!");
     * * REQUIRE(vaccine, "Vaccine cannot be a nullpointer!");
     * * REQUIRE(!vaccine->getType().empty(), "Vaccine type cannot be empty!");
     * * ENSURE(getVaccinsDelivered().at(vaccine->getType()) >= amount, "Didn't succesfully add the delivery to the simulationdata!");
     */
    void addDelivery(Vaccine* vaccine, unsigned int amount);
    /**
     * Adds an amount to the total amount of vaccinated inhabitants
     * @param amount: unsigned int; amount of vaccinated inhabitants to add
     * * REQUIRE(properlyInitialized(), "SimulationData wasn't initialized properly!");
     * * ENSURE(getAmountVaccinated() >= amount, "Didn't succesfully add the new amount of vaccinated inhabitatns!");
     */
    void addVaccinatedInhabitants(unsigned int amount);
    /**
     * Outputs the statistics to an output stream
     * @param stream: ostream; output stream to send the data to
     * REQUIRE(properlyInitialized(), "SimulationData wasn't initialized properly!");
     * * REQUIRE(stream.good(), "Stream to print statistics to is in an error state!");
     * * REQUIRE(stream.good(), "Printing the statistics ended up in an error state!");
     */
    void printStatistics(std::ostream &stream) const;
    /**
     * @return the total amount of vaccinated people
     *
     */
    unsigned int getAmountVaccinated() const;
    /**
     * @return the different vaccines and their amount that were delivered in total
     * * REQUIRE(properlyInitialized(), "SimulationData wasn't initialized properly!");
     */
    std::map<std::string, unsigned int>& getVaccinsDelivered();
    /**
     * Resets the simulationdata
     * REQUIRE(properlyInitialized(), "SimulationData wasn't initialized properly!");
     * ENSURE(getVaccinsDelivered().empty(), "SimulationData didn't reset well!");
     * ENSURE(getAmountVaccinated() == 0, "SimulationData didn't reset well!");
     */
    void reset();
    /**
     * Connects a center to a hub to indicate the hub sent a transportation to the center
     * @param hub: Hub*; hub the transportation came from
     * @param center: VaccinationCenter*; center to sent the transportation to
     * * REQUIRE(properlyInitialized(), "SimulationData wasn't initialized properly!");
     * * REQUIRE(hub, "Hub cannot be NULL!");
     * * REQUIRE(center, "Center cannot be NULL!");
     * * ENSURE(getTransportations().find(hub) == getTransportations().end(), "Hub should be added to the map!");
     */
    void addTransportation(Hub* hub, VaccinationCenter* center);
    /**
     * @return the transportations done the last run
     * * REQUIRE(properlyInitialized(), "SimulationData wasn't initialized properly!");
     */
    std::map<Hub*, std::vector<VaccinationCenter*> >& getTransportations();


private:
    const SimulationData* initCheck;

    std::map<std::string, unsigned int> vaccinsDelivered;
    unsigned int vaccinated;

    std::map<Hub*, std::vector<VaccinationCenter*> > transportations;

};


#endif //VACCIMULATOR_SIMULATIONDATA_H
