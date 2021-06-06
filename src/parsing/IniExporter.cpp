// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#include "IniExporter.h"
#include "../entities/VaccinationCenter.h"
#include "../entities/Hub.h"
#include "../../DesignByContract.h"

#define MINIMUM_BUILDING_WIDTH 0.5
#define MINIMUM_BUILDINGS 4

IniExporter::IniExporter(): initCheck(this), widthOfHub(0), widthOfVaccinationCenter(0), spaceBetweenVaccinationCenters(0), spaceBetweenHubs(0), iniFile() {
    ENSURE(properlyInitialized(), "IniExporter wasn't initialized properly!");
}

bool IniExporter::properlyInitialized() {
    return initCheck == this;
}

IniExporter::IniExporter(std::ofstream &out, std::vector<Hub *> &hubs, std::vector<VaccinationCenter *> &centers): initCheck(this), widthOfHub(0), widthOfVaccinationCenter(0), spaceBetweenVaccinationCenters(0), spaceBetweenHubs(0), hubs(hubs), centers(centers), iniFile(out) {
    ENSURE(properlyInitialized(), "IniExporter wasn't initialized properly!");
}

void IniExporter::widthOfObjects() {

    //define width of a hub in the animation
    if ((int) hubs.size() < MINIMUM_BUILDINGS) {
        widthOfHub = MINIMUM_BUILDING_WIDTH;
    } else {
        widthOfHub = ((double) MINIMUM_BUILDINGS) / (3.0 * ((double) hubs.size()) + 1.0);
    }

    //define width of a center in the animation
    if ((int) centers.size() < MINIMUM_BUILDINGS) {
        widthOfVaccinationCenter = MINIMUM_BUILDING_WIDTH;
    } else {
        widthOfVaccinationCenter = ((double) MINIMUM_BUILDINGS) / (3.0 * ((double) centers.size()) + 1.0);
    }
}

void IniExporter::spaceBetweenObjects() {

    //define space between consecutive hubs on the ground in animation
    if ((int) hubs.size() < MINIMUM_BUILDINGS) {
        spaceBetweenHubs = (2.0 - ((double) hubs.size() * MINIMUM_BUILDING_WIDTH)) / ((double) hubs.size() + 1.0);
    } else {
        spaceBetweenHubs = widthOfHub / 2.0;
    }

    //define space between consecutive vaccinationcenters on the ground in animation
    if ((int) centers.size() < MINIMUM_BUILDINGS) {
        spaceBetweenVaccinationCenters =
                (2.0 - (((double) centers.size()) * MINIMUM_BUILDING_WIDTH)) / (((double) centers.size()) + 1.0);
    } else {
        spaceBetweenVaccinationCenters = widthOfVaccinationCenter / 2.0;
    }
}


void IniExporter::generateVaccineBoxInCenter(int offset, int plan_of_vaccinationcenter, int nrFigures, int id) {
    iniFile << "\n[Figure" << nrFigures + offset << "]\ntype = \"Cube\"\nid = " << id <<
            "\nscale = " << widthOfVaccinationCenter / 6 <<
            "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = ("
            <<
            widthOfVaccinationCenter / 2.0 + spaceBetweenVaccinationCenters + plan_of_vaccinationcenter *
                                                                              (widthOfVaccinationCenter +
                                                                               spaceBetweenVaccinationCenters) +
            widthOfVaccinationCenter / 6 << ", "
            << 2.0 - widthOfVaccinationCenter / 2.0 + widthOfVaccinationCenter / 6 << ", "
            << widthOfVaccinationCenter / 6 <<
            ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
}


void IniExporter::generateVaccinationCenter(int plan_of_vaccinationcenter, int nrFigures, int id,
                                            int vaccin_boxes_in_center) {

    //specifications for body of VaccinationCenter, generated body for iniFile
    iniFile << "\n[Figure" << nrFigures << "]\ntype = \"Cube\"\nid = " << id <<
            "\nscale = " << widthOfVaccinationCenter / 2.0 <<
            "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = ("
            << widthOfVaccinationCenter / 2.0 + spaceBetweenVaccinationCenters +
                plan_of_vaccinationcenter *
                (widthOfVaccinationCenter + spaceBetweenVaccinationCenters) << ", "
            << 2.0 - widthOfVaccinationCenter / 2.0 <<
            ", " << widthOfVaccinationCenter / 2.0 + 0.1 * widthOfVaccinationCenter <<
            ")\ncolor = (1, 0, 0)\nobject = vaccinationcenter\n";

    //specifications for roof of VaccinationCenter, generated roof for iniFile
    iniFile << "\n[Figure" << nrFigures + 1 << "]\ntype = \"Cone\"\nid = " << id <<
            "\nscale = " << widthOfVaccinationCenter / 2.0 <<
            "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = ("
            << widthOfVaccinationCenter / 2.0 + spaceBetweenVaccinationCenters +
                plan_of_vaccinationcenter * (widthOfVaccinationCenter +
                                             spaceBetweenVaccinationCenters) << ", "
            << 2.0 - widthOfVaccinationCenter / 2.0 << ", " << widthOfVaccinationCenter +
                                                               0.1 * widthOfVaccinationCenter << ")\ncolor = ("
             << 1 - ((double) centers[plan_of_vaccinationcenter]->getVaccinationsDone() / (double)
                     centers[plan_of_vaccinationcenter]->getInhabitants()) << ", "
             << (double) centers[plan_of_vaccinationcenter]->getVaccinationsDone() /
                (double) centers[plan_of_vaccinationcenter]->getInhabitants()
             << ", 0)\nn = 20\nheight = 10.0\nobject = vaccinationcenter\n";
    for(int i = 1; i <= 5; i++) {
        if (vaccin_boxes_in_center >= i) {
            generateVaccineBoxInCenter(i+2, plan_of_vaccinationcenter, nrFigures, id);
        }
    }
}


void IniExporter::generateHub(int hub_idx, double center_position_of_hub, int nrFigures, int id, int vaccin_boxes_in_hub) {

    //specifications for body of Hub, generated body for iniFile
    iniFile << "\n[Figure" << nrFigures << "]\ntype = \"Cube\"\nid = " << id <<
            "\nscale = " << widthOfHub / 2.0 << "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = ("
            << center_position_of_hub
            << ", " << widthOfHub / 2.0 << ", " << widthOfHub / 2.0 + 0.1 * widthOfHub
             << ")\ncolor = (0, 1, 0)\nobject = hub\n";

    if (vaccin_boxes_in_hub >= 1) {
        /*  ___________________ (boxes locations)
         *  |        |        |
         *  |   box  |        |
         *  |________|        |
         *  |                 |
         *  |                 |
         *  |_________________|
         */
        iniFile << "\n[Figure" << nrFigures + 1 << "]\ntype = \"Cube\"\nid = " << id <<
                "\nscale = " << widthOfHub / 6.0 <<
                "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub + widthOfHub / 6.0
                << ", " << widthOfHub / 2.0 +
                           widthOfHub / 6.0
                 << ", " <<
                widthOfHub /
                6.0
                 << ")\ncolor = (0, 0, 1)\nobject = hub\n";
    }
    if (vaccin_boxes_in_hub >= 2) {
        /*  ___________________ (boxes locations)
         *  |        |        |
         *  |   box  |        |
         *  |________|        |
         *  |        |        |
         *  |   box  |        |
         *  |________|________|
         */
        iniFile << "\n[Figure" << nrFigures + 2 << "]\ntype = \"Cube\"\nid = " << id <<
                "\nscale = " << widthOfHub / 6.0 <<
                "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub + widthOfHub / 6.0
                << ", " << widthOfHub / 2.0 -
                           widthOfHub / 6.0
                 << ", " <<
                widthOfHub /
                6.0
                 << ")\ncolor = (0, 0, 1)\nobject = hub\n";
    }
    if (vaccin_boxes_in_hub >= 3) {
        /*  ___________________ (boxes locations)
         *  |        |        |
         *  |   box  |        |
         *  |________|________|
         *  |        |        |
         *  |   box  |   box  |
         *  |________|________|
         */
        iniFile << "\n[Figure" << nrFigures + 3 << "]\ntype = \"Cube\"\nid = " << id <<
                "\nscale = " << widthOfHub / 6.0 <<
                "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub - widthOfHub / 6.0
                << ", " << widthOfHub / 2.0 +
                           widthOfHub / 6.0
                 << ", " <<
                widthOfHub /
                6.0
                 << ")\ncolor = (0, 0, 1)\nobject = hub\n";
    }
    if (vaccin_boxes_in_hub >= 4) {
        /*  ___________________ (boxes locations)
         *  |        |        |
         *  |   box  |   box  |
         *  |________|________|
         *  |        |        |
         *  |   box  |   box  |
         *  |________|________|
         */
        iniFile << "\n[Figure" << nrFigures + 4 << "]\ntype = \"Cube\"\nid = " << id <<
                "\nscale = " << widthOfHub / 6.0 <<
                "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub - widthOfHub / 6.0
                << ", " << widthOfHub / 2.0 -
                           widthOfHub / 6.0
                 << ", " <<
                widthOfHub /
                6.0
                 << ")\ncolor = (0, 0, 1)\nobject = hub\n";
    }
    if (vaccin_boxes_in_hub == 5) {
        /*  ____________________ (boxes locations)
         *  |     ________     |
         *  | box |      | box |
         *  |_____|  box |_____|
         *  |     |______|     |
         *  |   box  |   box   |
         *  |________|_________|
         */
        iniFile << "\n[Figure" << nrFigures + 5 << "]\ntype = \"Cube\"\nid = " << id <<
                "\nscale = " << widthOfHub / 6.0 <<
                "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub <<
                ", " << widthOfHub / 2.0 << ", " << widthOfHub / 3 + widthOfHub / 6
                 << ")\ncolor = (0, 0, 1)\nobject = hub\n";
    }
}
