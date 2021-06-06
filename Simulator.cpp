// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include <fstream>
#include "Simulator.h"
#include "utils.h"
#include "DesignByContract.h"
#include "json/JObject.h"

#define HUB_SIZE 4
#define CENTER_SIZE 4
#define MAXIMUM_BOXES 5
#define BOX_PER_VACCINS_CENTER 1000
#define BOX_PER_VACCINS_HUB 10000
#define BOXES_CENTER_OFFSET 999
#define BOXES_HUB_OFFSET 9999

Simulator::Simulator(const unsigned int cycles): initCheck(this), daycount(0), cycles(cycles) {//, planned_hubs(cycles) {
    ENSURE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
}

void Simulator::importSimulation(const std::string& fileName) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been properly initialized!");
    REQUIRE(!fileName.empty(), "Filename cannot be empty!");
    JObject* json = xmlParser.parse(fileName);
    std::vector<JValue*> hubs_json = json->getValue("hubs")->asJArray()->getItems();
    std::vector<JValue*> centers_json = json->getValue("centra")->asJArray()->getItems();
    for (int i = 0; i < (int) centers_json.size(); i++) {
        centers.push_back(new VaccinationCenter());
        centers[i]->fromJSON(centers_json[i]->asJObject());
    }
    sort(centers.begin(), centers.end());
    for (int i = 0; i < (int) centers.size(); i++) {
        plan.getCenter2idx()[centers[i]] = i;
    }
    for (int i = 0; i < (int) hubs_json.size(); i++) {
        hubs.push_back(new Hub());
        hubs[i]->fromJSON(hubs_json[i]->asJObject());
        for (int k = 0; k < (int) hubs[i]->getVaccins().size(); k++) {
            unsigned int count = cycles/(hubs[i]->getVaccins()[k]->getRenewing()+1);
            for (unsigned int z = 0; z <= count; z++) {
                (hubs[i]->getVaccins()[k]->getDays()).push_back(0);
            }
            vaccins.push_back(hubs[i]->getVaccins()[k]);
        }
    }
    sort(vaccins.begin(), vaccins.end());
    for (int i = 0; i < (int) centers.size(); i++) {
        std::vector<std::pair<int,std::map<Vaccine*, int>>> elementt;
        plan.planned.push_back(elementt);
        for (int k = 0; k < cycles; k++) {
            std::pair<int, std::map<Vaccine*>> pt;
            plan.planned[i].push_back(pt);
        }
    }
    generatePlanning(hubs, centers, vaccins, cycles);
}

void Simulator::exportSimulation(const std::string& fileName) const {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(!fileName.empty(), "Filename cannot be empty!");
    std::ofstream file;
    file.open(fileName.c_str());
    assert(file.is_open());
    hub.toSummaryStream(file);
    hub.toProgressStream(file);
    file.close();
    ENSURE(!file.is_open(), "File wasn't closed properly!");
}

void Simulator::simulateDay(const unsigned int day) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(day < 0, "day cannot be negative!");
    REQUIRE(day+1 > cycles, "day cannot be bigger than total days defined in the simulation!");
    for (int i = 0; i < (int) vaccins.size(); i++) {
        if (day%vaccins[i]->getInterval() == 0) { //mss 1 dag later?
            vaccins[i]->getDynamicDelivery() += vaccins[i]->getDelivery();
        }
    }
    for (int center_kolom = 0; center_kolom < (int) centers.size(); center_kolom++) {
        centers[center_kolom]->getTempCapacity() = centers[center_kolom]->getCapacity();
        for (int k = 0; k < cycles; k++) {
            for (auto it = plan.planned[center_kolom][k].second.begin; it != plan.planned[center_kolom][k].second.end; it++) {
                if ((it->first->getTemperature() < 0) && (centers[center_kolom]->getTempCapacity()-it->second >= 0) {
                    centers[center_kolom]->getTempCapacity() -= it->second;
                    it->first->getDynamicDelivery() -= it->second;
                    centers[center_kolom]->getVaccinated() += it->second;
                    it->second = 0;
                }
            }
        }
    }

    for (int center_kolom = 0; center_kolom < (int) centers.size(); center_kolom++) {
        for (int k = 0; k < cycles; k++) {
            for (auto it = plan.planned[center_kolom][k].second.begin; it != plan.planned[center_kolom][k].second.end; it++) {
                if (centers[center_kolom]->getTempCapacity()-it->second >= 0) { //kan een vaccinatiecenter max "capacity" aantal vaccins vaccinere?
                    centers[center_kolom]->getTempCapacity() -= it->second;
                    it->first->getDynamicDelivery() -= it->second;
                    centers[center_kolom]->getVaccinated() += it->second;
                    it->second = 0;
                }
            }
        }
    }
}

void Simulator::run() {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(cycles != 0, "Cycles cannot be 0!");
    unsigned int lastDay = daycount + cycles, oldDaycount = daycount;
    for(; daycount < lastDay; daycount++){
        simulateDay(daycount);
    }
    ENSURE(daycount == oldDaycount + cycles, "Simulator didn't succesfully finish the right amount of cycles!");
}

bool Simulator::properlyInitialized() const {
    return initCheck == this;
}



void Simulator::widthOfObjects(double& width_of_hub, double& width_of_vaccinationcenter) const {

    //define width of a hub in the animation
    if ((int) hubs.size() < HUB_SIZE) {
        width_of_hub = 0.5;
    } else {
        width_of_hub = 4.0 / (3.0 * ((double) hubs.size()) + 1.0);
    }

    //define width of a center in the animation
    if ((int) centers.size() < CENTER_SIZE) {
        width_of_vaccinationcenter = 0.5;
    } else {
        width_of_vaccinationcenter = 4.0 / (3.0 * ((double) centers.size()) + 1.0);
    }
}

void Simulator::spaceBetweenObjects(double& space_between_vaccinationcenters, double& space_between_hubs, double width_of_hub, double width_of_vaccinationcenter) const {

    //define space between consecutive hubs on the ground in animation
    if ((int) hubs.size() < HUB_SIZE) {
        space_between_hubs = (2.0 - ((double) hubs.size() * 0.5))/((double) hubs.size() + 1.0);
    } else {
        space_between_hubs = width_of_hub/2.0;
    }

    //define space between consecutive vaccinationcenters on the ground in animation
    if ((int) centers.size()  < CENTER_SIZE) {
        space_between_vaccinationcenters = (2.0 - (((double) centers.size()) * 0.5))/(((double) centers.size()) + 1.0);
    } else {
        space_between_vaccinationcenters = width_of_vaccinationcenter/2.0;
    }
}




void Simulator::generateBodyOfVaccinationCenter(std::ofstream& ini_file, int plan_of_vaccinationcenter, int nrFigures, int id, double width_of_vaccinationcenter, double space_between_vaccinationcenters, int vaccin_boxes_in_center) const {

    //specifications for body of VaccinationCenter, generated body for ini_file
    ini_file << "\n[Figure" << nrFigures << "]\ntype = \"Cube\"\nid = " << id << "\nscale = " << width_of_vaccinationcenter/2.0 <<
    "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << width_of_vaccinationcenter/2.0 + space_between_vaccinationcenters +
    plan_of_vaccinationcenter *(width_of_vaccinationcenter + space_between_vaccinationcenters) << ", " << 2.0 - width_of_vaccinationcenter/2.0
    << ", " << width_of_vaccinationcenter/2.0 + 0.1*width_of_vaccinationcenter << ")\ncolor = (1, 0, 0)\nobject = vaccinationcenter\n";
}

void Simulator::generateRoofOfVaccinationCenter(std::ofstream& ini_file, int plan_of_vaccinationcenter, int nrFigures, int id, double width_of_vaccinationcenter, double space_between_vaccinationcenters, int vaccin_boxes_in_center) const {

    //specifications for roof of VaccinationCenter, generated roof for ini_file
    ini_file << "\n[Figure" << nrFigures + 1 << "]\ntype = \"Cone\"\nid = " << id << "\nscale = " << width_of_vaccinationcenter/2.0 <<
    "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << width_of_vaccinationcenter/2.0 + space_between_vaccinationcenters +
    plan_of_vaccinationcenter*(width_of_vaccinationcenter +space_between_vaccinationcenters) << ", " << 2.0 - width_of_vaccinationcenter/2.0
    << ", " << width_of_vaccinationcenter + 0.1*width_of_vaccinationcenter << ")\ncolor = (" << 1 - ((double) centers[plan_of_vaccinationcenter]->getVaccinationsDone()
    /(double)centers[plan_of_vaccinationcenter]->getInhabitants())) << ", " << (double) centers[plan_of_vaccinationcenter]->getVaccinationsDone()/
    (double) centers[plan_of_vaccinationcenter]->getInhabitants()) << ", 0)\nn = 20\nheight = 10.0\nobject = vaccinationcenter\n";
}

void Simulator::generateBodyOfHub(std::ofstream& ini_file, int hub_idx, double center_position_of_hub, int nrFigures, int id, double width_of_hub, double space_between_hubs, int vaccin_boxes_in_hub) const {

    //specifications for body of Hub, generated body for ini_file
    ini_file << "\n[Figure" << nrFigures << "]\ntype = \"Cube\"\nid = " << id << "\nscale = " << width_of_hub/2.0 <<
    "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub << ", " << width_of_hub/2.0 << ", "
    << width_of_hub/2.0 + 0.1*width_of_hub << ")\ncolor = (0, 1, 0)\nobject = hub\n";
}

void Simulator::generateBodyCar(std::ofstream& ini_file, int nrFigures, int id, double width_of_car, double width_of_hub, double width_of_wheel, double center_position_of_hub, std::set<unsigned int>::iterator center_idx_iterator) const {

    //specifications for body of the car, generated body for ini_file
    ini_file << "\n[Figure" << nrFigures << "]\ntype = \"Cube\"\nid = " << id << "\nscale = " << width_of_car/2 <<
    "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub << ", " << width_of_hub/2.0 <<
    ", " << width_of_car/2.0 + width_of_wheel << ")\ncolor = (1, 0.0784, 0.5764)\nobject = car\nvisit = " << *center_idx_iterator << "\n"; //+ 0.1*wheel_width
}




void Simulator::generateVaccinBoxesInVaccinationCenter(std::ofstream& ini_file, int plan_of_vaccinationcenter, int nrFigures, int id, double width_of_vaccinationcenter, double space_between_vaccinationcenters, int vaccin_boxes_in_center) const {

    if (vaccin_boxes_in_center >= 1) {
        /*  ___________________ (boxes locations) 1 vaccine_box in the vaccinationcenter
         *  |        |        |
         *  |   box  |        |
         *  |________|        |
         *  |                 |
         *  |                 |
         *  |_________________|
         */
        ini_file << "\n[Figure" << nrFigures + 2 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = " << width_of_vaccinationcenter/6
        << "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << width_of_vaccinationcenter/2.0 + space_between_vaccinationcenters +
        plan_of_vaccinationcenter*(width_of_vaccinationcenter + space_between_vaccinationcenters) + width_of_vaccinationcenter/6 << ", "
        << 2.0 - width_of_vaccinationcenter/2.0 + width_of_vaccinationcenter/6 << ", " << width_of_vaccinationcenter/6 << ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
    }
    if (vaccin_boxes_in_center >= 2) {
        /*  ___________________ (boxes locations) 2 vaccine_boxes in the vaccinationcenter
         *  |        |        |
         *  |   box  |        |
         *  |________|        |
         *  |        |        |
         *  |   box  |        |
         *  |________|________|
         */
        ini_file << "\n[Figure" << nrFigures + 3 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = " << width_of_vaccinationcenter/6 <<
        "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << width_of_vaccinationcenter/2.0 + space_between_vaccinationcenters +
        plan_of_vaccinationcenter*(width_of_vaccinationcenter + space_between_vaccinationcenters) + width_of_vaccinationcenter/6
        << ", " << 2.0 - width_of_vaccinationcenter/2.0 - width_of_vaccinationcenter/6 << ", " << width_of_vaccinationcenter/6 << ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
    }
    if (vaccin_boxes_in_center >= 3) {
        /*  ___________________ (boxes locations) 3 vaccine_boxes in the vaccinationcenter
         *  |        |        |
         *  |   box  |        |
         *  |________|________|
         *  |        |        |
         *  |   box  |   box  |
         *  |________|________|
         */
        ini_file << "\n[Figure" << nrFigures + 4 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = " << width_of_vaccinationcenter/6 <<
        "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << width_of_vaccinationcenter/2.0 + space_between_vaccinationcenters +
        plan_of_vaccinationcenter*(width_of_vaccinationcenter + space_between_vaccinationcenters) - width_of_vaccinationcenter/6
        << ", " << 2.0 - width_of_vaccinationcenter/2.0 + width_of_vaccinationcenter/6 << ", " << width_of_vaccinationcenter/6 << ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
    }
    if (vaccin_boxes_in_center >= 4) {
        /*  ___________________ (boxes locations) 4 vaccine_boxes in the vaccinationcenter
         *  |        |        |
         *  |   box  |   box  |
         *  |________|________|
         *  |        |        |
         *  |   box  |   box  |
         *  |________|________|
         */
        ini_file << "\n[Figure" << nrFigures + 5 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = " << width_of_vaccinationcenter/6 <<
        "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << width_of_vaccinationcenter/2.0 + space_between_vaccinationcenters +
        plan_of_vaccinationcenter*(width_of_vaccinationcenter + space_between_vaccinationcenters) - width_of_vaccinationcenter/6
        << ", " << 2.0 - width_of_vaccinationcenter/2.0 - width_of_vaccinationcenter/6 << ", " << width_of_vaccinationcenter/6 << ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
    }
    if (vaccin_boxes_in_center == 5) {
        /*  ____________________ (boxes locations) 5 vaccine_boxes in the vaccinationcenter
         *  |     ________     |
         *  | box |      | box |
         *  |_____|  box |_____|
         *  |     |______|     |
         *  |   box  |   box   |
         *  |________|_________|
         */
        ini_file << "\n[Figure" << nrFigures + 6 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = " << width_of_vaccinationcenter/6 <<
        "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << width_of_vaccinationcenter/2.0 + space_between_vaccinationcenters +
        plan_of_vaccinationcenter*(width_of_vaccinationcenter + space_between_vaccinationcenters) << ", " << 2.0 - width_of_vaccinationcenter/2.0 <<
        ", " << width_of_vaccinationcenter/3 + width_of_vaccinationcenter/6 << ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
    }
}

void Simulator::generateVaccinBoxesInHub(std::ofstream& ini_file, int hub_idx, double center_position_of_hub, int nrFigures, int id, double width_of_hub, double space_between_hubs, int vaccin_boxes_in_hub) const {

    if (vaccin_boxes_in_hub >= 1) {
        /*  ___________________ (boxes locations) 1 vaccine_box in the hub
         *  |        |        |
         *  |   box  |        |
         *  |________|        |
         *  |                 |
         *  |                 |
         *  |_________________|
         */
        ini_file << "\n[Figure" << nrFigures + 1 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = " << width_of_hub / 6.0 <<
        "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub + width_of_hub / 6.0 << ", " <<
        width_of_hub / 2.0 + width_of_hub / 6.0 << ", " << width_of_hub / 6.0 << ")\ncolor = (0, 0, 1)\nobject = hub\n";
    }
    if (vaccin_boxes_in_hub >= 2) {
        /*  ___________________ (boxes locations) 2 vaccine_boxes in the hub
         *  |        |        |
         *  |   box  |        |
         *  |________|        |
         *  |        |        |
         *  |   box  |        |
         *  |________|________|
         */
        ini_file << "\n[Figure" << nrFigures + 2 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = " << width_of_hub / 6.0 <<
        "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub + width_of_hub / 6.0 << ", " <<
        width_of_hub / 2.0 - width_of_hub / 6.0 << ", " << width_of_hub / 6.0 << ")\ncolor = (0, 0, 1)\nobject = hub\n";
    }
    if (vaccin_boxes_in_hub >= 3) {
        /*  ___________________ (boxes locations) 3 vaccine_boxes in the hub
         *  |        |        |
         *  |   box  |        |
         *  |________|________|
         *  |        |        |
         *  |   box  |   box  |
         *  |________|________|
         */
        ini_file << "\n[Figure" << nrFigures + 3 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = " << width_of_hub / 6.0 <<
        "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub - width_of_hub / 6.0 << ", " <<
        width_of_hub / 2.0 + width_of_hub / 6.0 << ", " << width_of_hub / 6.0 << ")\ncolor = (0, 0, 1)\nobject = hub\n";
    }
    if (vaccin_boxes_in_hub >= 4) {
        /*  ___________________ (boxes locations) 4 vaccine_boxes in the hub
         *  |        |        |
         *  |   box  |   box  |
         *  |________|________|
         *  |        |        |
         *  |   box  |   box  |
         *  |________|________|
         */
        ini_file << "\n[Figure" << nrFigures + 4 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = " << width_of_hub / 6.0 <<
        "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub - width_of_hub / 6.0 << ", " <<
        width_of_hub / 2.0 - width_of_hub / 6.0 << ", " << width_of_hub / 6.0 << ")\ncolor = (0, 0, 1)\nobject = hub\n";
    }
    if (vaccin_boxes_in_hub == 5) {
        /*  ____________________ (boxes locations) 5 vaccine_boxes in the hub
         *  |     ___|____     |
         *  | box |      | box |
         *  |_____|  box |_____|
         *  |     |______|     |
         *  |   box  |   box   |
         *  |________|_________|
         */
        ini_file << "\n[Figure" << nrFigures + 5 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = " << width_of_hub / 6.0 <<
        "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub << ", " << width_of_hub / 2.0 << ", " <<
        width_of_hub / 3 + width_of_hub / 6 << ")\ncolor = (0, 0, 1)\nobject = hub\n";
    }
}

void Simulator::generateVaccinBoxesInCar(std::ofstream& ini_file, int nrFigures, int id, double width_of_car, double width_of_hub, double width_of_wheel, double center_position_of_hub, std::set<unsigned int>::iterator center_idx_iterator) const {

    //specifications for the vaccin_boxes in the car (that it transports) for the ini_file

    //box 1
    ini_file << "\n[Figure" << nrFigures+1 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = " << width_of_car/6 <<
    "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub << ", " << width_of_hub/2.0 <<
    ", " << width_of_car/6 + width_of_car/3 + width_of_wheel << ")\ncolor = (1, 0.5, 0.2)\nvisit = " << *center_idx_iterator << "\n";

    //box 2
    ini_file << "\n[Figure" << nrFigures+2 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = " << width_of_car/6 <<
    "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub + width_of_car/6 << ", " <<
    width_of_hub/2.0 + width_of_car/6 << ", " << width_of_car/6 + width_of_wheel << ")\ncolor = (1, 0.5, 0.2)\nobject = car_box\nvisit = " << *center_idx_iterator << "\n";

    //box 3
    ini_file << "\n[Figure" << nrFigures+3 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = " << width_of_car/6 <<
    "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub + width_of_car/6 << ", " <<
    width_of_hub/2.0 - width_of_car/6 << ", " << width_of_car/6 + width_of_wheel << ")\ncolor = (1, 0.5, 0.2)\nobject = car_box\nvisit = " << *center_idx_iterator << "\n";

    //box 4
    ini_file << "\n[Figure" << nrFigures+4 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = " << width_of_car/6 <<
    "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub - width_of_car/6 << ", " <<
    width_of_hub/2.0 + width_of_car/6 << ", " << width_of_car/6 + width_of_wheel << ")\ncolor = (1, 0.5, 0.2)\nobject = car_box\nvisit = " << *center_idx_iterator << "\n";

    //box 5
    ini_file << "\n[Figure" << nrFigures+5 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = " << width_of_car/6 <<
    "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub - width_of_car/6 << ", " <<
    width_of_hub/2.0 - width_of_car/6 << ", " << width_of_car/6 + width_of_wheel << ")\ncolor = (1, 0.5, 0.2)\nobject = car_box\nvisit = " << *center_idx_iterator << "\n";
}

void Simulator::generateWheelsOfCar(std::ofstream& ini_file, int nrFigures, int id, double width_of_car, double width_of_hub, double width_of_wheel, double center_position_of_hub, std::set<unsigned int>::iterator center_idx_iterator) const {

    //specifications for the wheels of the car for the ini_file

    //wheel 1
    ini_file << "\n[Figure" << nrFigures+6 << "]\ntype = \"Sphere\"\nid = " << id << "\nscale = " << width_of_wheel/2 <<
    "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub + width_of_car/2 - width_of_wheel/2 <<
    ", " << width_of_hub/2.0 + width_of_car/2 - width_of_wheel << ", " << width_of_wheel/2 << ")\ncolor = (1, 1, 1)\nn = 5\nobject = car\nvisit = " << *center_idx_iterator << "\n";

    //wheel 2
    ini_file << "\n[Figure" << nrFigures+7 << "]\ntype = \"Sphere\"\nid = " << id << "\nscale = " << width_of_wheel/2 <<
    "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub + width_of_car/2 - width_of_wheel/2 <<
    ", " << width_of_hub/2.0 - width_of_car/2 + width_of_wheel << ", " << width_of_wheel/2 << ")\ncolor = (1, 1, 1)\nn = 5\nobject = car\nvisit = " << *center_idx_iterator << "\n";

    //wheel 3
    ini_file << "\n[Figure" << nrFigures+8 << "]\ntype = \"Sphere\"\nid = " << id << "\nscale = " << width_of_wheel/2 <<
    "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub - width_of_car/2 + width_of_wheel/2 <<
    ", " << width_of_hub/2.0 + width_of_car/2 - width_of_wheel << ", " << width_of_wheel/2 << ")\ncolor = (1, 1, 1)\nn = 5\nobject = car\nvisit = " << *center_idx_iterator << "\n";

    //wheel 4
    ini_file << "\n[Figure" << nrFigures+9 << "]\ntype = \"Sphere\"\nid = " << id << "\nscale = " << width_of_wheel/2 <<
    "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position_of_hub - width_of_car/2 + width_of_wheel/2 <<
    ", " << width_of_hub/2.0 - width_of_car/2 + width_of_wheel << ", " << width_of_wheel/2 << ")\ncolor = (1, 1, 1)\nn = 5\nobject = car\nvisit = " << *center_idx_iterator << "\n";
}



void Simulator::generateVaccinationCenter(std::ofstream& ini_file, int plan_of_vaccinationcenter, int nrFigures, int id, double width_of_vaccinationcenter, double space_between_vaccinationcenters, int vaccin_boxes_in_center) const {

    //generate body of the vaccinationCenter
    generateBodyOfVaccinationCenter(ini_file, plan_of_vaccinationcenter, nrFigures, id, width_of_vaccinationcenter, space_between_vaccinationcenters, vaccin_boxes_in_center);

    //generate roof of the vaccinationCenter
    generateRoofOfVaccinationCenter(ini_file, plan_of_vaccinationcenter, nrFigures, id, width_of_vaccinationcenter, space_between_vaccinationcenters, vaccin_boxes_in_center);

    //generate vaccin boxes in the vaccinationCenter
    generateVaccinBoxesInVaccinationCenter(ini_file, plan_of_vaccinationcenter, nrFigures, id, width_of_vaccinationcenter, space_between_vaccinationcenters, vaccin_boxes_in_center);
}

void Simulator::generateHub(std::ofstream& ini_file, int hub_idx, double center_position_of_hub, int nrFigures, int id, double width_of_hub, double space_between_hubs, int vaccin_boxes_in_hub) const {

    //generate body of the Hub
    generateBodyOfHub(ini_file, hub_idx, center_position_of_hub, nrFigures, id, width_of_hub, space_between_hubs, vaccin_boxes_in_hub);

    //generate vaccin boxes in the Hub
    generateVaccinBoxesInHub(ini_file, hub_idx, center_position_of_hub, nrFigures, id, width_of_hub, space_between_hubs, vaccin_boxes_in_hub);
}

void Simulator::generateCar(std::ofstream& ini_file, int nrFigures, int id, double width_of_car, double width_of_hub, double width_of_wheel, double center_position_of_hub, std::set<unsigned int>::iterator center_idx_iterator) const {

    //generate body of the car
    generateBodyCar(ini_file, nrFigures, id, width_of_car, width_of_hub, width_of_wheel, center_position_of_hub, center_idx_iterator);

    //generate vaccinBoxes in the car (to transport)
    generateVaccinBoxesInCar(ini_file, nrFigures, id, width_of_car, width_of_hub, width_of_wheel, center_position_of_hub, center_idx_iterator);

    //generate wheels for the car
    generateWheelsOfCar(ini_file, nrFigures, id, width_of_car, width_of_hub, width_of_wheel, center_position_of_hub, center_idx_iterator);
}


void Simulator::generate_animation() const {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");

    //variabelen
    double width_of_hub;
    double width_of_vaccinationcenter;
    double space_between_hubs;
    double space_between_vaccinationcenters;
    long long id = 0; //id per figure in ini file
    int nrFigures = 0; //number of figures present in ini file
    std::ofstream ini_file("frame" + std::to_string(daycount) + ".ini");

    //define width of centers and hubs
    widthOfObjects(width_of_hub, width_of_vaccinationcenter);

    //define space between consecutive hubs and consecutive centers
    spaceBetweenObjects(space_between_vaccinationcenters, space_between_hubs, width_of_hub, width_of_vaccinationcenter);

    //generate VaccinationCenters for ini_file
    for (int plan_of_vaccinationcenter = 0; plan_of_vaccinationcenter < (int) centers.size(); plan_of_vaccinationcenter++) {

        //define total number of vaccin_boxes in the centrum upon receiving from a hub
        int vaccin_boxes_in_center = std::min(MAXIMUM_BOXES,(plan.getPlanned()[plan_of_vaccinationcenter][daycount] + BOXES_CENTER_OFFSET)/BOX_PER_VACCINS_CENTER); //add total boxes to nrFigures

        //generate a string representation of a vaccinationcenter for the ini_file
        generateVaccinationCenter(ini_file, plan_of_vaccinationcenter, nrFigures, id, width_of_vaccinationcenter, space_between_vaccinationcenters, vaccin_boxes_in_center);

        // 2 is the number of figures that makes up a vaccinationCenter and vaccin_boxes_in_center are the number of figures representing the vaccin boxes in the center
        nrFigures += 2 + vaccin_boxes_in_center;
        id++;
    }

    id = 0;

    //generate hubs for ini_file
    for (int hub_idx = 0; hub_idx < ((int) hubs.size()); hub_idx++) {

        //center x-coordinate position of the hub in 3d space
        double center_position_of_hub = width_of_hub/2.0 + space_between_hubs + hub_idx*(width_of_hub + space_between_hubs);

        //total vaccin boxes present in hub
        int vaccin_boxes_in_hub = std::min(MAXIMUM_BOXES,(hubs[hub_idx]->getTotalvaccins() + BOXES_HUB_OFFSET)/BOX_PER_VACCINS_HUB);

        //generate a string representation of a hub for the ini_file
        generateHub(ini_file, hub_idx, center_position_of_hub, nrFigures, id, width_of_hub, space_between_hubs, vaccin_boxes_in_hub);

        // 1 is the number of figures that makes up a hub and vaccin_boxes_in_hub are the number of figures representing the vaccin boxes in the hub
        nrFigures += 1 + vaccin_boxes_in_hub;
        id++;

        //indices of VaccinationCenter objects in "centers" that are connected to the current hub
        std::set<unsigned int> centers_connected = planned_hubs[daycount][hubs[hub_idx]];

        //generate cars that go from a hub to a VaccinationCenter
        for (std::set<unsigned int>::iterator center_idx_iterator = centers_connected.begin(); center_idx_iterator != centers_connected.end(); center_idx_iterator++) {

            double width_of_car = std::min(width_of_hub/2.0, width_of_vaccinationcenter/2.0);
            double width_of_wheel = std::min(width_of_hub/2.0, width_of_vaccinationcenter/2.0)/4;

            //generate a string representation of a car for the ini_file
            generateCar(ini_file, nrFigures, id, width_of_car, width_of_hub, width_of_wheel, center_position_of_hub, center_idx_iterator);

            nrFigures += 10;
            id++;
        }
    }

    //generate the ground for the animation (a cube)
    ini_file << "\n[Figure" << nrFigures << "]\ntype = \"Cube\"\nid = " << id << "\nscale = 1\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (1, 1, -1)\ncolor = (0, 0, 1)\nobject = ground\n";

    //generate "GENERAL" section of the ini_file for general specifications
    ini_file << "[General]\ntype = \"Animation\"\nsize = 1024\neye = (20, 15, 50)\nbackgroundcolor = (0.0, 0.0, 0.0)\nnrFigures = " << nrFigures << "\nnrVaccinationCenters = " << (int) centers.size() << "\n";

    //close the file
    ini_file.close();
}
