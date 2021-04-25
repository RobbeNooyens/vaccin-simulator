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


Simulator::Simulator(const unsigned int cycles): initCheck(this), daycount(0), cycles(cycles) {
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
        for (unsigned int k = 0; k < cycles; k++) {
            (centers[i]->getCalender()).push_back({0, {}});
        }
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

void Simulator::simulateDay(unsigned int day) {
    for (int i = 0; i < (int) centers.size(); i++) {
        
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

//vaccins met temperatuur < 0 gaan eerst
//als temperatuur < 0 van een vaccin en het wordt bijgehouden in een centra => het moet weg

//code houdt niet rekening met lading van vaccin > capacity om te  gebruiken voor volgende dag
bool Simulator::is_valid(unsigned int k, int z, int i) {
    if (((k+vaccins[z]->getRenewing() < cycles) && (centers[i]->getCalender()[k+vaccins[z]->getRenewing()].first == centers[i]->getCapacity()))) {
        return false;
    }
    unsigned int c1 = vaccins[z]->getDays()[k/vaccins[z]->getInterval()] + vaccins[z]->getTransportation();
    unsigned int c2 = 0;
    if ((k+vaccins[z]->getRenewing() < cycles) && vaccins[z]->getRenewing()) {
        c2 = vaccins[z]->getDays()[(k+vaccins[z]->getRenewing())/vaccins[z]->getInterval()];
        c2 += ((k/vaccins[z]->getInterval()) != ((k+vaccins[z]->getRenewing())/vaccins[z]->getInterval())) ? 2*vaccins[z]->getTransportation() : vaccins[z]->getTransportation();
    }
    if ((c1 > (vaccins[z]->getDelivery() + (k/vaccins[z]->getInterval())*vaccins[z]->getDelivery())) || (c2 && (c2 > (vaccins[z]->getDelivery() + ((k+vaccins[z]->getRenewing())/vaccins[z]->getInterval())*vaccins[z]->getDelivery())))) {
        return false;
    }
    unsigned int count = cycles/(hubs[i]->getVaccins()[k]->getRenewing()+1);
    for (unsigned int vullen = (k/vaccins[z]->getInterval()); vullen <= count; vullen++) {
        vaccins[z]->getDays()[vullen] += (vaccins[z]->getRenewing() && (vullen >= (k+vaccins[z]->getRenewing())/vaccins[z]->getInterval())) ? 2*vaccins[z]->getTransportation() : vaccins[z]->getTransportation();
    }
    return true;
}

void Simulator::make_calender() {
    for (int i = 0; i < (int) centers.size(); i++) {
        bool test = false;
        unsigned int num = centers[i]->getInhabitants();
        for (unsigned int k = 0; k < cycles; k++) {
            if (num == 0) {
                break;
            }
            if (centers[i]->getCalender()[k].first || test) { //let ook op de einde positie met renewing
                continue;
            }
            for (int z = 0; z < (int) vaccins.size(); z++) {
                if (!vaccins[z]->getHub()->is_connected(centers[i]->getName()) || !is_valid(k,z,i)) {
                    continue;
                }
                unsigned int tt = std::min(centers[i]->getCapacity(), vaccins[z]->getTransportation());
                if ((k+vaccins[z]->getRenewing() < cycles) && vaccins[z]->getRenewing()) {
                    tt = std::min(tt, centers[i]->getCapacity()-centers[i]->getCalender()[k+vaccins[z]->getRenewing()].first);
                }
                centers[i]->getCalender()[k].second[vaccins[z]] += tt;
                centers[i]->getCalender()[k].first += tt;
                num -= std::min(num, tt);
                if (num == 0) {
                    break;
                }
                if ((k+vaccins[z]->getRenewing() < cycles) && vaccins[z]->getRenewing()) {
                    centers[i]->getCalender()[k+vaccins[z]->getRenewing()].second[vaccins[z]] += tt;
                    centers[i]->getCalender()[k+vaccins[z]->getRenewing()].first += tt;
                    num -= std::min(num, tt);
                }
                break;
            }
        }
    }
    for (int i = 0; i < (int) centers.size(); i++) {
        unsigned int num = centers[i]->getInhabitants();
        for (unsigned int k = 0; k < cycles; k++) {
            if (num == 0) {
                break;
            }
            for (int z = 0; z < (int) vaccins.size(); z++) {
                if (!vaccins[z]->getHub()->is_connected(centers[i]->getName())) {
                    continue;
                }
                while ((num != 0) && (centers[i]->getCalender()[k].first < centers[i]->getCapacity()) && is_valid(k,z,i)) {
                    unsigned int tt = std::min(centers[i]->getCapacity()-centers[i]->getCalender()[k].first, vaccins[z]->getTransportation());
                    if ((k+vaccins[z]->getRenewing() < cycles) && vaccins[z]->getRenewing()) {
                       tt = std::min(tt, centers[i]->getCapacity()-centers[i]->getCalender()[k+vaccins[z]->getRenewing()].first);
                    }
                    centers[i]->getCalender()[k].second[vaccins[z]] += tt;
                    centers[i]->getCalender()[k].first += tt;
                    num -= std::min(num, tt);
                    if (num == 0) {
                        break;
                    }
                    if ((k+vaccins[z]->getRenewing() < cycles) && vaccins[z]->getRenewing()) {
                        centers[i]->getCalender()[k+vaccins[z]->getRenewing()].second[vaccins[z]] += tt;
                        centers[i]->getCalender()[k+vaccins[z]->getRenewing()].first += tt;
                        num -= std::min(num, tt);
                    }                
                }
            }
        }
    }
}

//zet verhouding tussen R en r waarbij R > r;

void Simulator::generate_animation() const {
    double width_hub, width_center, hub_space, center_space;
    if ((int) hubs.size() < 4) {
        double width_hub = 0.5;
        double hub_space = (2.0 - ((double) hubs.size() * 0.5))/((double) hubs.size() + 1.0);
    } else {
        double width_hub = 4.0/(3.0*((double) hubs.size()) + 1.0);
        double hub_space = width_hub/2.0;
    }
    if ((int) centers.size() < 4) {
        double width_center = 0.5;
        double center_space = (2.0 - ((double) centers.size() * 0.5))/((double) centers.size() + 1.0);
    } else {
        double width_center = 4.0/(3.0*((double) centers.size()) + 1.0);
        double center_space = width_center/2.0;
    }
    //2*scale = width_hub;
    //2*scale = width_center;
    int nrFigures = (int) hubs.size() + (int) centers.size() + 1; //vermenigvuldig nog factoren met bv aantal objecten waaruit een vacinatiecentrum bestaat.
    // for (int i = 0; i < (int) hubs.size(); i++) {
    //     nrFigures += hubs[i]->get_connections();
    // }
    ofstream myfile; //verander de [Figure0] bij de eerste cube
    myfile.open("frame" + std::to_string(daycount) + ".ini"); //"animation" is hetzelfde als "ZBuffering" maar het genereert per dag meerdere fotos;
    myfile << "[GENERAL]\ntype = \"animation\"\nsize = 1024\neye = (x,y,z)\nbackgroundcolor = (0.0, 0.0, 0.0)\nnrFigures = " + std::to_string(nrFigures) + "\n";
    for (int i = 0; i < (int) centers.size(); i++) {
        myfile << "\n[Figure" + std::to_string(i) + "]\ntype = \"Cube\"\nscale = " + std::to_string(width_center/2.0) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + std::to_string(width_center/2.0 + center_space) + ", " + std::to_string(width_center/2.0) + ", 0)\ncolor = (1, 0, 0)\n"; //verander color naargelang de aantal gevacineerde inwoners;
    }
    for (int i = (int) centers.size(); i < ((int) hubs.size() + (int) centers.size()); i++) {
        myfile << "\n[Figure" + std::to_string(i) + "]\ntype = \"Cube\"\nscale = " + std::to_string(hub_center/2.0) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + std::to_string(width_hub/2.0 + hub_space) + ", " + std::to_string(width_hub/2.0) + ", 0)\ncolor = (0, 1, 0)\n";
    }
    myfile << "\n[Figure" + std::to_string(nrFigures-1) + "]\ntype = \"Cube\"\nscale = 1\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (1, 1, -1)\ncolor = (0, 0, 1)\n";
    myfile.close() //zet nog in ini file welke hub me welke centra connected is om autos te generere.
}