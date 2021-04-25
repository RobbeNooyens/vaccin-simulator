// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#ifndef VACCIN_SIMULATOR_VACCINATIONCENTER_H
#define VACCIN_SIMULATOR_VACCINATIONCENTER_H

#include <string>

class TiXmlElement;
class JObject;

class VaccinationCenter {
public:
    // Constructor
    /**
     * Default constructor
     * ENSURE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    VaccinationCenter();
    /**
     * VaccionationCenter parameter constructor
     * @param name: center's name
     * @param address: the address where the center is located
     * @param inhabitants: the amount of inhabitants that should be vaccinated in this center
     * @param capacity: the max amount of people that can be vaccinated here daily
     * ENSURE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */

    /**
     * Checks if the current object was initialized properly
     * @return bool; true if the initCheck pointer points to the current instance
     */
    bool properlyInitialized() const;

    // Simulation
    /**
     * Simulates the event of an arriving truck with vaccines.
     * @param vaccinCount: unsigned int; the amount of vaccines that are being delivered
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     * ENSURE(vaccins = oldVaccins + vaccinCount, "Vaccins aren't added succesfully!");
     */
    void transportationArrived(unsigned int vaccinCount);
    /**
     * Simulate the situation where the highest possible amount of inhabitants are being vaccinated
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     * ENSURE(vaccinated = oldVaccinated + vaccinsToUse, "Vaccinated count didn't increase.");
     * ENSURE(vaccins = oldVaccins - vaccinsToUse, "Vaccins count didn't decrease.");
     */
    void vaccinateInhabitants();

    // Getters
    /**
     * @return string; name of the vaccinationcenter
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    std::string getName() const;
    /**
     * @return string; address of the vaccinationcenter
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    std::string getAddress() const;
    /**
     * @return unsigned int; total amount of people that should be vaccinated in this center
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    unsigned int getInhabitants() const;
    /**
     * @return unsigned int; the maximum amount of people that can be vaccinated during one day in this center
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    unsigned int getCapacity() const;
    /**
     * @return unsigned int; the amount of vaccines in stock
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    unsigned int getVaccins() const;
    /**
     * @return unsigned int; amount of inhabitants who are already vaccinated
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    unsigned int getVaccinationsDone() const;
    /**
     * @return unsigned int; the amount of non-vaccinated inhabitants
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     */
    unsigned int getVaccinationsLeft() const;

    // IO
    /**
     * Load a VaccinationCenter from a Json object
     * @param json: the Json object representing a VaccinationCenter
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     * REQUIRE(json != NULL, "Json can't be NULL!");
     * REQUIRE(json->contains("adres"), "VaccinationCenter JSON should contain field 'adres'");
     * REQUIRE(json->contains("capaciteit"), "VaccinationCenter JSON should contain field 'capaciteit'");
     * REQUIRE(json->contains("inwoners"), "VaccinationCenter JSON should contain field 'inwoners'");
     * REQUIRE(json->contains("naam"), "VaccinationCenter JSON should contain field 'naam'");
     */
    void fromJSON(JObject* json);
    /**
     * Sends the center represented as a string to the given stream
     * @param stream: The stream that should be used
     * REQUIRE(properlyInitialized(), "VaccinationCenter object hasn't been initialized properly!");
     * REQUIRE(outStream != NULL, "Output stream cannot be NULL!");
     * REQUIRE(outStream.good(), "Output stream contains error flags!");
     * ENSURE(outStream.good(), "Failed to write to output stream!");
     */
    void toSummaryStream(std::ostream& stream) const;
    void toProgressStream(std::ostream& stream) const;

    bool operator<(const VaccinationCenter& s) const;

    std::vector<std::pair<unsigned int, unordered_map<Vaccine*, unsigned int>>>& getCalender() const;

private:
    std::vector<std::pair<unsigned int, unordered_map<Vaccine*, unsigned int>>> vaccine_calender;
    // Initialization
    const VaccinationCenter* initCheck;

    // Metadata
    std::string name;
    std::string address;
    unsigned int vaccins;
    unsigned int inhabitants;
    unsigned int totalInhabitants;
    unsigned int vaccinated;
    unsigned int capacity;
};


#endif //VACCIN_SIMULATOR_VACCINATIONCENTER_H
