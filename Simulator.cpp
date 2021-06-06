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


Simulator::Simulator(const unsigned int cycles): initCheck(this), daycount(0), cycles(cycles), planned_hubs(cycles) {
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
        center2idx[centers[i]->getName()] = i;
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

void Simulator::generate_animation() const {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    double width_hub, width_center, hub_space, center_space;
    long long id = 0;
    if ((int) hubs.size() < 4) {
        width_hub = 0.5;
        hub_space = (2.0 - ((double) hubs.size() * 0.5))/((double) hubs.size() + 1.0);
    } else {
        width_hub = 4.0/(3.0*((double) hubs.size()) + 1.0);
        hub_space = width_hub/2.0;
    }
    if ((int) centers.size() < 4) {
        width_center = 0.5;
        center_space = (2.0 - (((double) centers.size()) * 0.5))/(((double) centers.size()) + 1.0);
    } else {
        width_center = 4.0/(3.0*((double) centers.size()) + 1.0);
        center_space = width_center/2.0;
    }
    int nrFigures = 0;
    ofstream myfile("frame" + to_string(daycount) + ".ini");
    //"animation" is hetzelfde als "ZBuffering" maar het genereert per dag meerdere fotos;
    for (int i = 0; i < (int) centers.size(); i++) {
        myfile << "\n[Figure" + to_string(nrFigures) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_center/2.0) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(width_center/2.0 + center_space + i*(width_center + center_space)) + ", " + to_string(2.0 - width_center/2.0) + ", " + to_string(width_center/2.0 + 0.1*width_center) + ")\ncolor = (1, 0, 0)\nobject = vaccinationcenter\n";
        myfile << "\n[Figure" + to_string(nrFigures + 1) + "]\ntype = \"Cone\"\nid = " + to_string(id) + "\nscale = " + to_string(width_center/2.0) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(width_center/2.0 + center_space + i*(width_center + center_space)) + ", " + to_string(2.0 - width_center/2.0) + ", " + to_string(width_center + 0.1*width_center) + ")\ncolor = (" + to_string(1 - ((double) centers[i]->getVaccinationsDone()/(double) centers[i]->getInhabitants())) + ", " + to_string((double) centers[i]->getVaccinationsDone()/(double) centers[i]->getInhabitants()) + ", 0)\nn = 20\nheight = 10.0\nobject = vaccinationcenter\n";
        int vaccin_boxes = std::min(5,(centers[i]->getCalender()[daycount] + 999)/1000); //add total boxes to nrFigures
        if (vaccin_boxes >= 1) {
            myfile << "\n[Figure" + to_string(nrFigures + 2) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_center/6) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(width_center/2.0 + center_space + i*(width_center + center_space) + width_center/6) + ", " + to_string(2.0 - width_center/2.0 + width_center/6) + ", " + to_string(width_center/6) + ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
        }
        if (vaccin_boxes >= 2) {
            myfile << "\n[Figure" + to_string(nrFigures + 3) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_center/6) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(width_center/2.0 + center_space + i*(width_center + center_space) + width_center/6) + ", " + to_string(2.0 - width_center/2.0 - width_center/6) + ", " + to_string(width_center/6) + ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
        }
        if (vaccin_boxes >= 3) {
            myfile << "\n[Figure" + to_string(nrFigures + 4) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_center/6) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(width_center/2.0 + center_space + i*(width_center + center_space) - width_center/6) + ", " + to_string(2.0 - width_center/2.0 + width_center/6) + ", " + to_string(width_center/6) + ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
        }
        if (vaccin_boxes >= 4) {
            myfile << "\n[Figure" + to_string(nrFigures + 5) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_center/6) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(width_center/2.0 + center_space + i*(width_center + center_space) - width_center/6) + ", " + to_string(2.0 - width_center/2.0 - width_center/6) + ", " + to_string(width_center/6) + ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
        }
        if (vaccin_boxes == 5) {
            myfile << "\n[Figure" + to_string(nrFigures + 6) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_center/6) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(width_center/2.0 + center_space + i*(width_center + center_space)) + ", " + to_string(2.0 - width_center/2.0) + ", " + to_string(width_center/3 + width_center/6) + ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
        }
        nrFigures += 2 + vaccin_boxes;
        id++;
    }
    id = 0;
    for (int i = 0; i < ((int) hubs.size()); i++) {
        double center_position = width_hub/2.0 + hub_space + i*(width_hub + hub_space);
        myfile << "\n[Figure" + to_string(nrFigures) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_hub/2.0) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position) + ", " + to_string(width_hub/2.0) + ", " + to_string(width_hub/2.0 + 0.1*width_hub) + ")\ncolor = (0, 1, 0)\nobject = hub\n";
        int vaccin_boxes = std::min(5,(hubs[i]->getTotalvaccins() + 9999)/10000);
        if (vaccin_boxes >= 1) {
            myfile << "\n[Figure" + to_string(nrFigures + 1) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_hub/6.0) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position + width_hub/6.0) + ", " + to_string(width_hub/2.0 + width_hub/6.0) + ", " + to_string(width_hub/6.0) + ")\ncolor = (0, 0, 1)\nobject = hub\n";
        if (vaccin_boxes >= 2) {
            myfile << "\n[Figure" + to_string(nrFigures + 2) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_hub/6.0) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position + width_hub/6.0) + ", " + to_string(width_hub/2.0 - width_hub/6.0) + ", " + to_string(width_hub/6.0) + ")\ncolor = (0, 0, 1)\nobject = hub\n";
        }
        if (vaccin_boxes >= 3) {
            myfile << "\n[Figure" + to_string(nrFigures + 3) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_hub/6.0) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position - width_hub/6.0) + ", " + to_string(width_hub/2.0 + width_hub/6.0) + ", " + to_string(width_hub/6.0) + ")\ncolor = (0, 0, 1)\nobject = hub\n";
        }
        if (vaccin_boxes >= 4) {
            myfile << "\n[Figure" + to_string(nrFigures + 4) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_hub/6.0) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position - width_hub/6.0) + ", " + to_string(width_hub/2.0 - width_hub/6.0) + ", " + to_string(width_hub/6.0) + ")\ncolor = (0, 0, 1)\nobject = hub\n";
        }
        if (vaccin_boxes == 5) {
            myfile << "\n[Figure" + to_string(nrFigures + 5) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_hub/6.0) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position) + ", " + to_string(width_hub/2.0) + ", " + to_string(width_hub/3 + width_hub/6) + ")\ncolor = (0, 0, 1)\nobject = hub\n";
        }
        nrFigures += 1 + vaccin_boxes;
        id++;
        std::set<unsigned int> centers_connected = planned_hubs[daycount][hubs[i]];
        for (auto it = centers_connected.begin(); it != centers_connected.end(); it++) {
            double car_width = std::min(width_hub/2.0, width_center/2.0);
            double wheel_width = std::min(width_hub/2.0, width_center/2.0)/4;
            myfile << "\n[Figure" + to_string(nrFigures) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(car_width/2) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position) + ", " + to_string(width_hub/2.0) + ", " + to_string(car_width/2.0 + wheel_width) + ")\ncolor = (1, 0.0784, 0.5764)\nobject = car\nvisit = " + to_string(*it) + "\n"; //+ 0.1*wheel_width
            myfile << "\n[Figure" + to_string(nrFigures+1) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(car_width/6) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position) + ", " + to_string(width_hub/2.0) + ", " + to_string(car_width/6 + car_width/3 + wheel_width) + ")\ncolor = (1, 0.5, 0.2)\nvisit = " + to_string(*it) + "\n";
            myfile << "\n[Figure" + to_string(nrFigures+2) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(car_width/6) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position + car_width/6) + ", " + to_string(width_hub/2.0 + car_width/6) + ", " + to_string(car_width/6 + wheel_width) + ")\ncolor = (1, 0.5, 0.2)\nobject = car_box\nvisit = " + to_string(*it) + "\n";
            myfile << "\n[Figure" + to_string(nrFigures+3) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(car_width/6) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position + car_width/6) + ", " + to_string(width_hub/2.0 - car_width/6) + ", " + to_string(car_width/6 + wheel_width) + ")\ncolor = (1, 0.5, 0.2)\nobject = car_box\nvisit = " + to_string(*it) + "\n";
            myfile << "\n[Figure" + to_string(nrFigures+4) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(car_width/6) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position - car_width/6) + ", " + to_string(width_hub/2.0 + car_width/6) + ", " + to_string(car_width/6 + wheel_width) + ")\ncolor = (1, 0.5, 0.2)\nobject = car_box\nvisit = " + to_string(*it) + "\n";
            myfile << "\n[Figure" + to_string(nrFigures+5) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(car_width/6) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position - car_width/6) + ", " + to_string(width_hub/2.0 - car_width/6) + ", " + to_string(car_width/6 + wheel_width) + ")\ncolor = (1, 0.5, 0.2)\nobject = car_box\nvisit = " + to_string(*it) + "\n";

            myfile << "\n[Figure" + to_string(nrFigures+6) + "]\ntype = \"Sphere\"\nid = " + to_string(id) + "\nscale = " + to_string(wheel_width/2) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position + car_width/2 - wheel_width/2) + ", " + to_string(width_hub/2.0 + car_width/2 - wheel_width) + ", " + to_string(wheel_width/2) + ")\ncolor = (1, 1, 1)\nn = 5\nobject = car\nvisit = " + to_string(*it) + "\n";
            myfile << "\n[Figure" + to_string(nrFigures+7) + "]\ntype = \"Sphere\"\nid = " + to_string(id) + "\nscale = " + to_string(wheel_width/2) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position + car_width/2 - wheel_width/2) + ", " + to_string(width_hub/2.0 - car_width/2 + wheel_width) + ", " + to_string(wheel_width/2) + ")\ncolor = (1, 1, 1)\nn = 5\nobject = car\nvisit = " + to_string(*it) + "\n";
            myfile << "\n[Figure" + to_string(nrFigures+8) + "]\ntype = \"Sphere\"\nid = " + to_string(id) + "\nscale = " + to_string(wheel_width/2) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position - car_width/2 + wheel_width/2) + ", " + to_string(width_hub/2.0 + car_width/2 - wheel_width) + ", " + to_string(wheel_width/2) + ")\ncolor = (1, 1, 1)\nn = 5\nobject = car\nvisit = " + to_string(*it) + "\n";
            myfile << "\n[Figure" + to_string(nrFigures+9) + "]\ntype = \"Sphere\"\nid = " + to_string(id) + "\nscale = " + to_string(wheel_width/2) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position - car_width/2 + wheel_width/2) + ", " + to_string(width_hub/2.0 - car_width/2 + wheel_width) + ", " + to_string(wheel_width/2) + ")\ncolor = (1, 1, 1)\nn = 5\nobject = car\nvisit = " + to_string(*it) + "\n";
            nrFigures += 10;
            id++;
        }
    }
    myfile << "\n[Figure" + to_string(nrFigures) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = 1\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (1, 1, -1)\ncolor = (0, 0, 1)\nobject = ground\n";
    myfile << "[General]\ntype = \"Animation\"\nsize = 1024\neye = (20, 15, 50)\nbackgroundcolor = (0.0, 0.0, 0.0)\nnrFigures = " + to_string(nrFigures) + "\nnrVaccinationCenters = " + to_string((int) centers.size()) + "\n";
    myfile.close();
}
