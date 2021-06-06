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
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
}

std::map<Vaccine*, unsigned int> Planning::getDistribution(unsigned int day, VaccinationCenter* center) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    return planned[center2idx[center]][day].second;
}

bool Planning::properlyInitialized() {
    return initCheck == this;
}

std::vector<std::vector<std::pair<int,std::map<Vaccine*, int>>>> Planning::getPlanned() const {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    return planned;
}

//als temperatuur < 0 van een vaccin en het wordt bijgehouden in een centra => het moet weg

bool Simulator::is_valid(unsigned int day, Vaccine* vaccine, Center* center, std::vector<Vaccine*>& vaccins, std::vector<VaccinationCenter*>& centers, std::vector<Hub*>& hubs, unsigned int cycles) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    if (((day+vaccine->getRenewing() < cycles) && (planned[center2idx[center]][day+vaccine->getRenewing()].first == center->getCapacity()))) {
        return false;
    }
    unsigned int c1 = vaccine->getDays()[day/vaccine->getInterval()] + vaccine->getTransportation();
    unsigned int c2 = 0;
    if ((day+vaccine->getRenewing() < cycles) && vaccine->getRenewing()) {
        c2 = vaccine->getDays()[(day+vaccine->getRenewing())/vaccine->getInterval()];
        c2 += ((day/vaccine->getInterval()) != ((day+vaccine->getRenewing())/vaccine->getInterval())) ? 2*vaccine->getTransportation() : vaccine->getTransportation();
    }
    if ((c1 > (vaccine->getDelivery() + (day/vaccine->getInterval())*vaccine->getDelivery())) || (c2 && (c2 > (vaccine->getDelivery() + ((day+vaccine->getRenewing())/vaccine->getInterval())*vaccine->getDelivery())))) {
        return false;
    }
    unsigned int count = cycles/(hubs[center2idx[center]]->getVaccins()[day]->getRenewing()+1);
    for (unsigned int vullen = (day/vaccine->getInterval()); vullen <= count; vullen++) {
        vaccine->getDays()[vullen] += (vaccine->getRenewing() && (vullen >= (day+vaccine->getRenewing())/vaccine->getInterval())) ? 2*vaccine->getTransportation() : vaccine->getTransportation();
    }
    ENSURE(!hubs.empty(), "vector of hubs cannot be empty!");
    ENSURE(!centers.empty(), "vector of centers cannot be empty!");
    ENSURE(!vaccins.empty(), "vector of vaccins cannot be empty!");
    ENSURE(cycles != 0, "total cycles in simulation can be 0!");
    return true;
}

void generatePlanning(std::vector<Hub*>& hubs, std::vector<VaccinationCenter*>& centers, std::vector<Vaccine*>& vaccins, unsigned int cycles) {
    REQUIRE(properlyInitialized(), "Planning object hasn't been initialized properly!");
    for (int center_idx = 0; center_idx < (int) centers.size(); center_idx++) {
        bool test = false;
        unsigned int TotalInhabitantsInCenter = centers[center_idx]->getInhabitants();
        for (unsigned int day = 0; day < cycles; day++) {
            if (TotalInhabitantsInCenter == 0) {
                break;
            }
            if (planned[center_idx][day].first || test) { //let ook op de einde positie met renewing
                continue;
            }
            for (int vaccine_idx = 0; vaccine_idx < (int) vaccins.size(); vaccine_idx++) {
                if (!vaccins[vaccine_idx]->getHub()->is_connected(centers[center_idx]->getName()) || !is_valid(day,vaccine_idx,center_idx, vaccins, centers, hubs, cycles)) {
                    continue;
                }
                unsigned int tt = std::min(centers[center_idx]->getCapacity(), vaccins[vaccine_idx]->getTransportation());
                if ((day+vaccins[vaccine_idx]->getRenewing() < cycles) && vaccins[vaccine_idx]->getRenewing()) {
                    tt = std::min(tt, centers[center_idx]->getCapacity()-planned[center_idx][day+vaccins[vaccine_idx]->getRenewing()].first);
                }
                planned[center_idx][day].second[vaccins[vaccine_idx]] += tt;
                planned[center_idx][day].first += tt;
                TotalInhabitantsInCenter -= std::min(TotalInhabitantsInCenter, tt);
                //planned_hubs[day][vaccins[vaccine_idx]->getHub()].insert(center_idx);
                if (TotalInhabitantsInCenter == 0) {
                    break;
                }
                if ((day+vaccins[vaccine_idx]->getRenewing() < cycles) && vaccins[vaccine_idx]->getRenewing()) {
                    planned[center_idx][day+vaccins[vaccine_idx]->getRenewing()].second[vaccins[vaccine_idx]] += tt;
                    planned[center_idx][day+vaccins[vaccine_idx]->getRenewing()].first += tt;
                    //planned_hubs[day+vaccins[vaccine_idx]->getRenewing()][vaccins[vaccine_idx]->getHub()].insert(center_idx);
                    TotalInhabitantsInCenter -= std::min(TotalInhabitantsInCenter, tt);
                }
                break;
            }
        }
    }
    for (int center_idx = 0; center_idx < (int) centers.size(); center_idx++) {
        unsigned int TotalInhabitantsInCenter = centers[center_idx]->getInhabitants();
        for (unsigned int day = 0; day < cycles; day++) {
            if (TotalInhabitantsInCenter == 0) {
                break;
            }
            for (int vaccine_idx = 0; vaccine_idx < (int) vaccins.size(); vaccine_idx++) {
                if (!vaccins[vaccine_idx]->getHub()->is_connected(centers[center_idx]->getName())) {
                    continue;
                }
                while ((TotalInhabitantsInCenter != 0) && (planned[center_idx][day].first < centers[center_idx]->getCapacity()) && is_valid(day,vaccine_idx,center_idx, vaccins, centers, hubs, cycles)) {
                    unsigned int tt = std::min(centers[center_idx]->getCapacity()-planned[center_idx][day].first, vaccins[vaccine_idx]->getTransportation());
                    if ((day+vaccins[vaccine_idx]->getRenewing() < cycles) && vaccins[vaccine_idx]->getRenewing()) {
                       tt = std::min(tt, centers[center_idx]->getCapacity()-planned[center_idx][day+vaccins[vaccine_idx]->getRenewing()].first);
                    }
                    planned[center_idx][day].second[vaccins[vaccine_idx]] += tt;
                    planned[center_idx][day].first += tt;
                    //planned_hubs[day//][vaccins[vaccine_idx]->getHub()].insert(center_idx);
                    TotalInhabitantsInCenter -= std::min(TotalInhabitantsInCenter, tt);
                    if (TotalInhabitantsInCenter == 0) {
                        break;
                    }
                    if ((day+vaccins[vaccine_idx]->getRenewing() < cycles) && vaccins[vaccine_idx]->getRenewing()) {
                        planned[center_idx][day+vaccins[vaccine_idx]->getRenewing()].second[vaccins[vaccine_idx]] += tt;
                        planned[center_idx][day+vaccins[vaccine_idx]->getRenewing()].first += tt;
                        //planned_hubs[day+vaccins[vaccine_idx]->getRenewing()][vaccins[vaccine_idx]->getHub()].insert(center_idx);
                        TotalInhabitantsInCenter -= std::min(TotalInhabitantsInCenter, tt);
                    }                
                }
            }
        }
    }
    ENSURE(!hubs.empty(), "vector of hubs cannot be empty!");
    ENSURE(!centers.empty(), "vector of centers cannot be empty!");
    ENSURE(!vaccins.empty(), "vector of vaccins cannot be empty!");
    ENSURE(cycles != 0, "total cycles in simulation can be 0!");
}
