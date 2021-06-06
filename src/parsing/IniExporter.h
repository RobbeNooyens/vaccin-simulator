// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#ifndef VACCIMULATOR_INIEXPORTER_H
#define VACCIMULATOR_INIEXPORTER_H

// TODO: documentation

#include <vector>
#include <fstream>

class Hub;
class VaccinationCenter;

class IniExporter {
public:
    /**
     * Default constructor
     * * ENSURE(properlyInitialized(), "IniExporter wasn't initialized properly!");
     */
    IniExporter();
    /**
     * Paramter constructor
     * @param out
     * @param hubs
     * @param centers
     * * ENSURE(properlyInitialized(), "IniExporter wasn't initialized properly!");
     */
    IniExporter(std::ofstream &out, std::vector<Hub*> &hubs, std::vector<VaccinationCenter*> &centers);
    bool properlyInitialized();
    void widthOfObjects();
    void spaceBetweenObjects();
    void generateVaccineBoxInCenter(int offset, int plan_of_vaccinationcenter, int nrFigures, int id);
    void generateVaccinationCenter(int plan_of_vaccinationcenter, int nrFigures, int id, int vaccin_boxes_in_center);
    void generateHub(int hub_idx, double center_position_of_hub, int nrFigures, int id, int vaccin_boxes_in_hub);
private:
    const IniExporter* initCheck;
    double widthOfHub, widthOfVaccinationCenter;
    double spaceBetweenVaccinationCenters, spaceBetweenHubs;
    std::vector<Hub*> hubs;
    std::vector<VaccinationCenter*> centers;

    std::ofstream iniFile;
};


#endif //VACCIMULATOR_INIEXPORTER_H
