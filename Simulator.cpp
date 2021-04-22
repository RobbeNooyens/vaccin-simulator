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

bool Simulator::is_valid(unsigned int k, int z, int i) {
    if (((k+vaccins[z]->getRenewing() < cycles) && (centers[i]->getCalender()[k+vaccins[z]->getRenewing()].first + vaccins[z]->getTransportation() > centers[i]->getCapacity()))) {
        return false;
    }
    vaccins[z]->getDays()[k/vaccins[z]->getInterval()] += vaccins[z]->getTransportation();
    if ((k+vaccins[z]->getRenewing() < cycles) && vaccins[z]->getRenewing()) {
        vaccins[z]->getDays()[(k+vaccins[z]->getRenewing())/vaccins[z]->getInterval()] += vaccins[z]->transportation;
    }
    if ((vaccins[z]->getDays()[k/vaccins[z]->getInterval()] > (vaccins[z]->getDelivery() + (k/vaccins[z]->getInterval())*vaccins[z]->getDelivery())) || (vaccins[z]->getDays()[(k+vaccins[z]->getRenewing())/vaccins[z]->getInterval()] > (vaccins[z]->getDelivery() + ((k+vaccins[z]->getRenewing())/vaccins[z]->getInterval())*vaccins[z]->getDelivery()))) {
        vaccins[z]->getDays()[k/vaccins[z]->getInterval()] -= vaccins[z]->getTransportation();
        if ((k+vaccins[z]->getRenewing() < cycles) && vaccins[z]->getRenewing()) {
            vaccins[z]->getDays()[(k+vaccins[z]->getRenewing())/vaccins[z]->getInterval()] -= vaccins[z]->getTransportation();
        }
        return false;
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
                centers[i]->getCalender()[k].first += vaccins[z]->getTransportation();
                centers[i]->getCalender()[k].second[vaccins[z]] += 1;
                num -= std::min(num, vaccins[z]->getTransportation());
                if (num == 0) {
                    break;
                }
                if ((k+vaccins[z]->getRenewing() < cycles) && vaccins[z]->getRenewing()) {
                    centers[i]->getCalender()[k+vaccins[z]->getRenewing()].first += vaccins[z]->getTransportation();
                    centers[i]->getCalender()[k+vaccins[z]->getRenewing()].second[vaccins[z]] += 1;
                    num -= std::min(num, vaccins[z]->getTransportation());
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
                    centers[i]->getCalender()[k].first += vaccins[z]->getTransportation();
                    centers[i]->getCalender()[k].second[vaccins[z]] += 1;
                    num -= std::min(num, vaccins[z]->getTransportation());
                    if (num == 0) {
                        break;
                    }
                    if ((k+vaccins[z]->getRenewing() < cycles) && vaccins[z]->getRenewing()) {
                        centers[i]->getCalender()[k+vaccins[z]->getRenewing()].first += vaccins[z]->getTransportation();
                        centers[i]->getCalender()[k+vaccins[z]->getRenewing()].second[vaccins[z]] += 1;
                        num -= std::min(num, vaccins[z]->getTransportation());
                    }                
                }
            }
        }
    }
}