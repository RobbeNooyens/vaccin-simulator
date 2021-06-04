// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 2.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include <vector>
#include "Planning.h"
#include "../DesignByContract.h"
#include "Hub.h"

Planning::Planning(): initCheck(this) {
    ENSURE(properlyInitialized(), "Planning object hasn't been initialized properly!");
}

Planning::~Planning() {

}

bool Planning::properlyInitialized() {
    return initCheck == this;
}

//vaccins met temperatuur < 0 gaan eerst
//als temperatuur < 0 van een vaccin en het wordt bijgehouden in een centra => het moet weg

//code houdt niet rekening met lading van vaccin > capacity om te  gebruiken voor volgende dag
bool Simulator::is_valid(unsigned int k, int z, int i, std::vector<Vaccine*>& vaccins, std::vector<VaccinationCenter*>& centers, std::vector<Hub*>& hubs, unsigned int cycles) {
    if (((k+vaccins[z]->getRenewing() < cycles) && (planned[i][k+vaccins[z]->getRenewing()].first == centers[i]->getCapacity()))) {
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

void generatePlanning(std::vector<Hub*>& hubs, std::vector<VaccinationCenter*>& centers, std::vector<Vaccine*>& vaccins, unsigned int cycles) {
    REQUIRE(properlyInitialized(), "Planning object hasn't been initialized properly!");
    for (int i = 0; i < (int) centers.size(); i++) {
        bool test = false;
        unsigned int num = centers[i]->getInhabitants();
        for (unsigned int k = 0; k < cycles; k++) {
            if (num == 0) {
                break;
            }
            if (planned[i][k].first || test) { //let ook op de einde positie met renewing
                continue;
            }
            for (int z = 0; z < (int) vaccins.size(); z++) {
                if (!vaccins[z]->getHub()->is_connected(centers[i]->getName()) || !is_valid(k,z,i, vaccins, centers, hubs, cycles)) {
                    continue;
                }
                unsigned int tt = std::min(centers[i]->getCapacity(), vaccins[z]->getTransportation());
                if ((k+vaccins[z]->getRenewing() < cycles) && vaccins[z]->getRenewing()) {
                    tt = std::min(tt, centers[i]->getCapacity()-planned[i][k+vaccins[z]->getRenewing()].first);
                }
                planned[i][k].second[vaccins[z]] += tt;
                planned[i][k].first += tt;
                num -= std::min(num, tt);
                planned_hubs[k][vaccins[z]->getHub()].insert(i);
                if (num == 0) {
                    break;
                }
                if ((k+vaccins[z]->getRenewing() < cycles) && vaccins[z]->getRenewing()) {
                    planned[i][k+vaccins[z]->getRenewing()].second[vaccins[z]] += tt;
                    planned[i][k+vaccins[z]->getRenewing()].first += tt;
                    planned_hubs[k+vaccins[z]->getRenewing()][vaccins[z]->getHub()].insert(i);
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
                while ((num != 0) && (planned[i][k].first < centers[i]->getCapacity()) && is_valid(k,z,i, vaccins, centers, hubs, cycles)) {
                    unsigned int tt = std::min(centers[i]->getCapacity()-planned[i][k].first, vaccins[z]->getTransportation());
                    if ((k+vaccins[z]->getRenewing() < cycles) && vaccins[z]->getRenewing()) {
                       tt = std::min(tt, centers[i]->getCapacity()-planned[i][k+vaccins[z]->getRenewing()].first);
                    }
                    planned[i][k].second[vaccins[z]] += tt;
                    planned[i][k].first += tt;
                    planned_hubs[k][vaccins[z]->getHub()].insert(i);
                    num -= std::min(num, tt);
                    if (num == 0) {
                        break;
                    }
                    if ((k+vaccins[z]->getRenewing() < cycles) && vaccins[z]->getRenewing()) {
                        planned[i][k+vaccins[z]->getRenewing()].second[vaccins[z]] += tt;
                        planned[i][k+vaccins[z]->getRenewing()].first += tt;
                        planned_hubs[k+vaccins[z]->getRenewing()][vaccins[z]->getHub()].insert(i);
                        num -= std::min(num, tt);
                    }                
                }
            }
        }
    }
}
