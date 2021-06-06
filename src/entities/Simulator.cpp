// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#include <fstream>
#include <set>
#include "Simulator.h"
#include "../utilities/utils.h"
#include "../../DesignByContract.h"
#include "../json/JObject.h"
#include "../json/JValue.h"
#include "../json/JArray.h"
#include "../json/JKeys.h"
#include "Planning.h"

#define C_ITERATE(type, iteratable, name) for(type::const_iterator name = iteratable.begin(); name != iteratable.end(); name++)
#define ITERATE(type, iteratable, name) for(type::iterator name = iteratable.begin(); name != iteratable.end(); name++)

// Constructors

Simulator::Simulator(): initCheck(this), initialState(NULL), daycount(0), statisticsOutputStream(NULL), transportationOutputStream(NULL), vaccinationOutputStream(NULL) {
    ENSURE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
}

Simulator::~Simulator() {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    delete initialState;
}

bool Simulator::properlyInitialized() const {
    return initCheck == this;
}

// IO

void Simulator::importSimulation(const std::string &fileName, std::ostream &errorStream) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been properly initialized!");
    REQUIRE(!fileName.empty(), "Filename cannot be empty!");
    delete initialState;
    ParseErrors errors;
    initialState = XMLParser::parse(fileName, errorStream, errors);
    if(!errors.empty())
        throw std::runtime_error("An error occured during parsing!");
    fromJSON(initialState);
}

void Simulator::exportSimulationSummary(std::ostream &out) const {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(out.good(), "Output stream should be writeable!");
    C_ITERATE(std::vector<Hub*>, hubs, hub) {
        (*hub)->toSummaryStream(out);
    }
    out << std::endl;
    C_ITERATE(std::vector<VaccinationCenter*>, centers, center) {
        (*center)->toSummaryStream(out);
    }
    ENSURE(out.good(), "Stream encountered an error during writing!");
}

void Simulator::exportSimulationProgress(std::ostream &out) const {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(out.good(), "Output stream should be writeable!");
    C_ITERATE(std::vector<VaccinationCenter*>, centers, center) {
        (*center)->toProgressStream(out);
    }
    ENSURE(out.good(), "Stream encountered an error during writing!");
}

void Simulator::fromJSON(JObject *json) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(json->contains(SIMULATION_CENTERS), StringUtil::concat("Can't load Simulator from JSON with missing field ", SIMULATION_CENTERS).c_str());
    REQUIRE(json->contains(SIMULATION_HUBS), StringUtil::concat("Can't load Simulator from JSON with missing field ", SIMULATION_HUBS).c_str());
    REQUIRE(ParseUtil::isConsistent(json), "Simulation JSON should be consistent!");
    JValues centersJSON = json->getValue(SIMULATION_CENTERS)->asJArray()->getItems();
    ITERATE(JValues, centersJSON, center) {
        VaccinationCenter* c = new VaccinationCenter();
        c->fromJSON((*center)->asJObject());
        c->setOutputStream(vaccinationOutputStream);
        centers.push_back(c);
    }
    JValues hubsJSON = json->getValue(SIMULATION_HUBS)->asJArray()->getItems();
    ITERATE(JValues, hubsJSON, hub) {
        Hub* h = new Hub();
        h->fromJSON((*hub)->asJObject(), centers);
        h->setOutputStream(transportationOutputStream);
        hubs.push_back(h);
    }
    ENSURE(!hubs.empty(), "Couldn't load hubs properly!");
    ENSURE(!centers.empty(), "Couldn't load centers properly!");
}

// Simulation controls

void Simulator::run(const unsigned int cycles) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(cycles != 0, "Cycles cannot be 0!");
    REQUIRE(isConsistent(), "Simulation needs to be consistent to run!");
    unsigned int lastDay = daycount + cycles, oldDaycount = daycount;
    while(daycount < lastDay){
        // Deliver vaccines to the hub if expected and transport vaccines to the centers
        ITERATE(std::vector<Hub*>, hubs, hub)(*hub)->simulateDay(daycount, statistics);
        // Vaccinate inhabitants (should happen here to prevent double vaccinations)
        ITERATE(VaccinationCenters, centers, center)(*center)->vaccinateInhabitants(daycount, statistics);
        if(statisticsOutputStream)
            statistics.printStatistics(*statisticsOutputStream);
        daycount++;
    }
    ENSURE(daycount == oldDaycount + cycles, "Simulator didn't succesfully finish the right amount of cycles!");
}

void Simulator::runEfficient(unsigned int cycles) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(cycles != 0, "Cycles cannot be 0!");
    REQUIRE(isConsistent(), "Simulation needs to be consistent to run!");
    Planning planning = Planning();
    Vaccines vaccines;
    ITERATE(std::vector<Hub*>, hubs, h) {
        Vaccines hubVaccines = (*h)->getVaccines();
        ITERATE(Vaccines, hubVaccines, vaccine) {
            vaccines.push_back(*vaccine);
        }
    }
    planning.generatePlanning(hubs, centers, vaccines, cycles);

    unsigned int lastDay = daycount + cycles;
    while(daycount < lastDay) {
        // Deliver vaccines to the hub if expected and transport vaccines to the centers
        ITERATE(std::vector<Hub*>, hubs, hub) {
            (*hub)->simulateDelivery(daycount, statistics);
            (*hub)->distributeEfficient(daycount, planning);
        }
        // Vaccinate inhabitants (should happen here to prevent double vaccinations)
        ITERATE(VaccinationCenters, centers, center)(*center)->vaccinateInhabitants(daycount, statistics);
        daycount++;
    }
}

bool Simulator::isConsistent() const {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    // There's at least 1 hub
    bool consistent = hubs.size() >= 1;
    // There's at least 1 center
    consistent = consistent && centers.size() >= 1;
    // All hubs are consistent
    C_ITERATE(std::vector<Hub*>, hubs, hub)
        consistent = consistent && (*hub)->isConsistent();
    // All centers are connected to at least one hub
    C_ITERATE(std::vector<VaccinationCenter*>, centers, center)
        consistent = consistent && (*center)->isConnectedToHub();
    return consistent;
}

void Simulator::reset() {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    ITERATE(std::vector<Hub*>, hubs, hub)
        delete *hub;
    ITERATE(std::vector<VaccinationCenter*>, centers, center)
        delete *center;
    hubs.clear();
    centers.clear();
    daycount = 0;
    fromJSON(initialState);
}

void Simulator::setStatisticsStream(std::ostream *stats) {
    this->statisticsOutputStream = stats;
}

void Simulator::setTransportationStream(std::ostream* transport) {
    this->transportationOutputStream = transport;
    ITERATE(std::vector<Hub*>, hubs, hub) {
        (*hub)->setOutputStream(transport);
    }
}

void Simulator::setVaccinationsStream(std::ostream *vaccinations) {
    this->vaccinationOutputStream = vaccinations;
    ITERATE(VaccinationCenters, centers, center) {
        (*center)->setOutputStream(vaccinations);
    }
}

void Simulator::setInitialState(JObject *json) {
    initialState = json;
}

VaccinationCenters &Simulator::getCenters() {
    return centers;
}

std::vector<Hub *> Simulator::getHubs() {
    return hubs;
}

void Simulator::exportSimulationIniFile(std::ostream &out) const {
    REQUIRE(properlyInitialized(), "Object hasn't been initialized properly!");
    REQUIRE(out.good(), "Output stream should be writeable!");

    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    double width_hub, width_center, hub_space, center_space;
    long long id = 0;
    if ((int) hubs.size() < 4) {
        width_hub = 0.5;
        hub_space = (2.0 - ((double) hubs.size() * 0.5))/((double) hubs.size() +  1.0);
    } else {
        width_hub = 4.0/(3.0*((double) hubs.size()) +  1.0);
        hub_space = width_hub/2.0;
    }
    if ((int) centers.size() < 4) {
        width_center = 0.5;
        center_space = (2.0 - (((double) centers.size()) * 0.5))/(((double) centers.size()) +  1.0);
    } else {
        width_center = 4.0/(3.0*((double) centers.size()) +  1.0);
        center_space = width_center/2.0;
    }
    int nrFigures = 0;
    //"animation" is hetzelfde als "ZBuffering" maar het genereert per dag meerdere fotos;
    for (int i = 0; i < (int) centers.size(); i++) {
        out << "\n[Figure" << nrFigures << "]\ntype = \"Cube\"\nid = " << id << "\nscale = " << width_center / 2.0 << "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << width_center / 2.0 + center_space + i * (width_center + center_space) << ", " << 2.0 - width_center / 2.0 << ", " << width_center / 2.0 + 0.1 * width_center << ")\ncolor = (1, 0, 0)\nobject = vaccinationcenter\n";
        out << "\n[Figure" << nrFigures + 1 <<  "]\ntype = \"Cone\"\nid = " << id <<  "\nscale = " << width_center / 2.0 <<  "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << width_center / 2.0 + center_space + i * (width_center + center_space) <<  ", " << 2.0 - width_center / 2.0 <<  ", " << width_center + 0.1 * width_center <<  ")\ncolor = (" << 1 - ((double) centers[i]->getVaccinationsDone() / (double) centers[i]->getInhabitants()) <<  ", " << (double) centers[i]->getVaccinationsDone() / (double) centers[i]->getInhabitants() <<  ", 0)\nn = 20\nheight = 10.0\nobject = vaccinationcenter\n";
        // TODO: fix getCalendar
        int vaccin_boxes = 5;//std::min(5,(centers[i]->getCalender()[daycount] + 999)/1000); //add total boxes to nrFigures
        if (vaccin_boxes >= 1) {
            out << "\n[Figure" << nrFigures + 2 <<  "]\ntype = \"Cube\"\nid = " << id <<  "\nscale = " << width_center / 6 <<  "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << width_center / 2.0 + center_space + i * (width_center + center_space) +  width_center / 6 <<  ", " << 2.0 - width_center / 2.0 + width_center / 6 <<  ", " << width_center / 6 <<  ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
        }
        if (vaccin_boxes >= 2) {
            out << "\n[Figure" << nrFigures + 3 <<  "]\ntype = \"Cube\"\nid = " << id <<  "\nscale = " << width_center / 6 <<  "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << width_center / 2.0 + center_space + i * (width_center + center_space) +  width_center / 6 <<  ", " << 2.0 - width_center / 2.0 - width_center / 6 <<  ", " << width_center / 6 <<  ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
        }
        if (vaccin_boxes >= 3) {
            out << "\n[Figure" << nrFigures + 4 <<  "]\ntype = \"Cube\"\nid = " << id <<  "\nscale = " << width_center / 6 <<  "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << width_center / 2.0 + center_space + i * (width_center + center_space) - width_center / 6 <<  ", " << 2.0 - width_center / 2.0 + width_center / 6 <<  ", " << width_center / 6 <<  ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
        }
        if (vaccin_boxes >= 4) {
            out << "\n[Figure" << nrFigures + 5 <<  "]\ntype = \"Cube\"\nid = " << id <<  "\nscale = " << width_center / 6 <<  "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << width_center / 2.0 + center_space + i * (width_center + center_space) - width_center / 6 <<  ", " << 2.0 - width_center / 2.0 - width_center / 6 <<  ", " << width_center / 6 <<  ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
        }
        if (vaccin_boxes == 5) {
            out << "\n[Figure" << nrFigures + 6 <<  "]\ntype = \"Cube\"\nid = " << id <<  "\nscale = " << width_center / 6 <<  "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << width_center / 2.0 + center_space + i * (width_center + center_space) <<  ", " << 2.0 - width_center / 2.0 <<  ", " << width_center / 3 + width_center / 6 <<  ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
        }
        nrFigures += 2 + vaccin_boxes;
        id++;
    }
    id = 0;
    for (int i = 0; i < ((int) hubs.size()); i++) {
        double center_position = width_hub / 2.0 + hub_space + i * (width_hub + hub_space);
        out << "\n[Figure" << nrFigures << "]\ntype = \"Cube\"\nid = " << id << "\nscale = " << width_hub / 2.0
                << "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position << ", " << width_hub / 2.0
                << ", " << width_hub / 2.0 + 0.1 * width_hub << ")\ncolor = (0, 1, 0)\nobject = hub\n";
        int vaccin_boxes = std::min(5, (int) (hubs[i]->getTotalvaccins() + 9999) / 10000);
        if (vaccin_boxes >= 1) {
            out << "\n[Figure" << nrFigures + 1 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = "
                    << width_hub / 6.0 << "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = ("
                    << center_position + width_hub / 6.0 << ", " << width_hub / 2.0 + width_hub / 6.0 << ", "
                    << width_hub / 6.0 << ")\ncolor = (0, 0, 1)\nobject = hub\n";
            if (vaccin_boxes >= 2) {
                out << "\n[Figure" << nrFigures + 2 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = "
                        << width_hub / 6.0 << "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = ("
                        << center_position + width_hub / 6.0 << ", " << width_hub / 2.0 - width_hub / 6.0 << ", "
                        << width_hub / 6.0 << ")\ncolor = (0, 0, 1)\nobject = hub\n";
            }
            if (vaccin_boxes >= 3) {
                out << "\n[Figure" << nrFigures + 3 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = "
                        << width_hub / 6.0 << "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = ("
                        << center_position - width_hub / 6.0 << ", " << width_hub / 2.0 + width_hub / 6.0 << ", "
                        << width_hub / 6.0 << ")\ncolor = (0, 0, 1)\nobject = hub\n";
            }
            if (vaccin_boxes >= 4) {
                out << "\n[Figure" << nrFigures + 4 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = "
                        << width_hub / 6.0 << "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = ("
                        << center_position - width_hub / 6.0 << ", " << width_hub / 2.0 - width_hub / 6.0 << ", "
                        << width_hub / 6.0 << ")\ncolor = (0, 0, 1)\nobject = hub\n";
            }
            if (vaccin_boxes == 5) {
                out << "\n[Figure" << nrFigures + 5 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = "
                        << width_hub / 6.0 << "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position
                        << ", " << width_hub / 2.0 << ", " << width_hub / 3 + width_hub / 6
                        << ")\ncolor = (0, 0, 1)\nobject = hub\n";
            }
            nrFigures += 1 + vaccin_boxes;
            id++;
            // TODO: connected centers
//            std::set<unsigned int> centers_connected = planned_hubs[daycount][hubs[i]];
//            ITERATE(std::set<unsigned int>, centers_connected, it) {
//                double car_width = std::min(width_hub / 2.0, width_center / 2.0);
//                double wheel_width = std::min(width_hub / 2.0, width_center / 2.0) / 4;
//                iniFile << "\n[Figure" << nrFigures << "]\ntype = \"Cube\"\nid = " << id << "\nscale = "
//                        << car_width / 2 << "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position
//                        << ", " << width_hub / 2.0 << ", " << car_width / 2.0 + wheel_width
//                        << ")\ncolor = (1, 0.0784, 0.5764)\nobject = car\nvisit = " << *it << "\n"; //+ 0.1*wheel_width
//                iniFile << "\n[Figure" << nrFigures + 1 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = "
//                        << car_width / 6 << "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" << center_position
//                        << ", " << width_hub / 2.0 << ", " << car_width / 6 + car_width / 3 + wheel_width
//                        << ")\ncolor = (1, 0.5, 0.2)\nvisit = " << *it << "\n";
//                iniFile << "\n[Figure" << nrFigures + 2 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = "
//                        << car_width / 6 << "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = ("
//                        << center_position + car_width / 6 << ", " << width_hub / 2.0 + car_width / 6 << ", "
//                        << car_width / 6 + wheel_width << ")\ncolor = (1, 0.5, 0.2)\nobject = car_box\nvisit = " << *it
//                        << "\n";
//                iniFile << "\n[Figure" << nrFigures + 3 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = "
//                        << car_width / 6 << "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = ("
//                        << center_position + car_width / 6 << ", " << width_hub / 2.0 - car_width / 6 << ", "
//                        << car_width / 6 + wheel_width << ")\ncolor = (1, 0.5, 0.2)\nobject = car_box\nvisit = " << *it
//                        << "\n";
//                iniFile << "\n[Figure" << nrFigures + 4 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = "
//                        << car_width / 6 << "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = ("
//                        << center_position - car_width / 6 << ", " << width_hub / 2.0 + car_width / 6 << ", "
//                        << car_width / 6 + wheel_width << ")\ncolor = (1, 0.5, 0.2)\nobject = car_box\nvisit = " << *it
//                        << "\n";
//                iniFile << "\n[Figure" << nrFigures + 5 << "]\ntype = \"Cube\"\nid = " << id << "\nscale = "
//                        << car_width / 6 << "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = ("
//                        << center_position - car_width / 6 << ", " << width_hub / 2.0 - car_width / 6 << ", "
//                        << car_width / 6 + wheel_width << ")\ncolor = (1, 0.5, 0.2)\nobject = car_box\nvisit = " << *it
//                        << "\n";
//
//                iniFile << "\n[Figure" << nrFigures + 6 << "]\ntype = \"Sphere\"\nid = " << id << "\nscale = "
//                        << wheel_width / 2 << "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = ("
//                        << center_position + car_width / 2 - wheel_width / 2 << ", "
//                        << width_hub / 2.0 + car_width / 2 - wheel_width << ", " << wheel_width / 2
//                        << ")\ncolor = (1, 1, 1)\nn = 5\nobject = car\nvisit = " << *it << "\n";
//                iniFile << "\n[Figure" << nrFigures + 7 << "]\ntype = \"Sphere\"\nid = " << id << "\nscale = "
//                        << wheel_width / 2 << "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = ("
//                        << center_position + car_width / 2 - wheel_width / 2 << ", "
//                        << width_hub / 2.0 - car_width / 2 + wheel_width << ", " << wheel_width / 2
//                        << ")\ncolor = (1, 1, 1)\nn = 5\nobject = car\nvisit = " << *it << "\n";
//                iniFile << "\n[Figure" << nrFigures + 8 << "]\ntype = \"Sphere\"\nid = " << id << "\nscale = "
//                        << wheel_width / 2 << "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = ("
//                        << center_position - car_width / 2 + wheel_width / 2 << ", "
//                        << width_hub / 2.0 + car_width / 2 - wheel_width << ", " << wheel_width / 2
//                        << ")\ncolor = (1, 1, 1)\nn = 5\nobject = car\nvisit = " << *it << "\n";
//                iniFile << "\n[Figure" << nrFigures + 9 << "]\ntype = \"Sphere\"\nid = " << id << "\nscale = "
//                        << wheel_width / 2 << "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = ("
//                        << center_position - car_width / 2 + wheel_width / 2 << ", "
//                        << width_hub / 2.0 - car_width / 2 + wheel_width << ", " << wheel_width / 2
//                        << ")\ncolor = (1, 1, 1)\nn = 5\nobject = car\nvisit = " << *it << "\n";
//                nrFigures += 10;
//                id++;
//            }
        }
        out << "\n[Figure" << nrFigures << "]\ntype = \"Cube\"\nid = " << id
                << "\nscale = 1\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (1, 1, -1)\ncolor = (0, 0, 1)\nobject = ground\n";
        out
                << "[General]\ntype = \"Animation\"\nsize = 1024\neye = (20, 15, 50)\nbackgroundcolor = (0.0, 0.0, 0.0)\nnrFigures = "
                << nrFigures << "\nnrVaccinationCenters = " << (int) centers.size() << "\n";
    }
    ENSURE(out.good(), "Stream encountered an error during writing!");
}
