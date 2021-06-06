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
    REQUIRE(errorStream.good(), "Errorstream is in an errorstate!");
    delete initialState;
    ParseErrors errors;
    initialState = XMLParser::parse(fileName, errorStream, errors);
    if(!errors.empty())
        throw std::runtime_error("An error occured during parsing!");
    fromJSON(initialState);
    ENSURE(ParseUtil::isConsistent(getInitialState()), "Initialstate json should be consistent!");
    ENSURE(isConsistent(), "Simulator should be valid!");
}

void Simulator::exportSimulationSummary(std::ostream &out) const {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(out.good(), "Output stream should be writeable!");
    REQUIRE(isConsistent(), "Simulator should be consistent!");
    C_ITERATE(std::vector<Hub*>, hubs, hub) {
        (*hub)->toSummaryStream(out);
    }
    out << std::endl;
    C_ITERATE(std::vector<VaccinationCenter*>, centers, center) {
        (*center)->toSummaryStream(out);
    }
    ENSURE(out.good(), "Stream encountered an error during writing!");
    ENSURE(isConsistent(), "Simulator should be consistent after writing to output stream!");
}

void Simulator::exportSimulationProgress(std::ostream &out) const {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(out.good(), "Output stream should be writeable!");
    REQUIRE(isConsistent(), "Simulator should be consistent!");
    C_ITERATE(std::vector<VaccinationCenter*>, centers, center) {
        (*center)->toProgressStream(out);
    }
    ENSURE(out.good(), "Stream encountered an error during writing!");
    ENSURE(isConsistent(), "Simulator should be consistent after writing to output stream!");
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
    ENSURE(!getHubs().empty(), "Couldn't load hubs properly!");
    ENSURE(!getCenters().empty(), "Couldn't load centers properly!");
    ENSURE(isConsistent(), "Simulation should be consistent after reading json!");
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
    ENSURE(getDayCount() == oldDaycount + cycles, "Simulator didn't succesfully finish the right amount of cycles!");
    ENSURE(isConsistent(), "Simulation needs to be consistent after running!");
}

void Simulator::runEfficient(unsigned int cycles) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(cycles != 0, "Cycles cannot be 0!");
    REQUIRE(isConsistent(), "Simulation needs to be consistent to run!");
    unsigned int oldDaycount = daycount;
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

    ENSURE(getDayCount() == oldDaycount + cycles, "Simulator didn't succesfully finish the right amount of cycles!");
    ENSURE(isConsistent(), "Simulation needs to be consistent after running!");
}

bool Simulator::isConsistent() const {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    // There's at least 1 hub
    bool result = hubs.size() >= 1;
    // There's at least 1 center
    result = result && centers.size() >= 1;
    // All hubs are result
    C_ITERATE(std::vector<Hub*>, hubs, hub)
        result = result && (*hub)->isConsistent();
    // All centers are connected to at least one hub
    C_ITERATE(std::vector<VaccinationCenter*>, centers, center)
        result = result && (*center)->isConnectedToHub();
    return result;
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
    ENSURE(isConsistent(), "Simulator should be consistent after reset!");
}

void Simulator::setStatisticsStream(std::ostream *stats) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    this->statisticsOutputStream = stats;
}

void Simulator::setTransportationStream(std::ostream* transport) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    this->transportationOutputStream = transport;
    ITERATE(std::vector<Hub*>, hubs, hub) {
        (*hub)->setOutputStream(transport);
    }
}

void Simulator::setVaccinationsStream(std::ostream *vaccinations) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    this->vaccinationOutputStream = vaccinations;
    ITERATE(VaccinationCenters, centers, center) {
        (*center)->setOutputStream(vaccinations);
    }
}

void Simulator::setInitialState(JObject *json) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    initialState = json;
}

VaccinationCenters &Simulator::getCenters() {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    return centers;
}

std::vector<Hub *> Simulator::getHubs() {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    return hubs;
}

JObject *Simulator::getInitialState() {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    return initialState;
}

unsigned int Simulator::getDayCount() {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    return daycount;
}

void Simulator::exportSimulationIniFile(std::ostream &out) const {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");

    IniExporter iniExporter;

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

    for (int plan_of_vaccinationcenter = 0; plan_of_vaccinationcenter < (int) centers.size(); plan_of_vaccinationcenter++) {

        //define total number of vaccin_boxes in the centrum upon receiving from a hub
        int vaccin_boxes_in_center = std::min(5,(plan.getPlanned()[plan_of_vaccinationcenter][daycount] + 999)/1000); //add total boxes to nrFigures

        //generate a string representation of a vaccinationcenter for the iniFile
        generateVaccinationCenter(ini_file, plan_of_vaccinationcenter, nrFigures, id, width_of_vaccinationcenter, space_between_vaccinationcenters, vaccin_boxes_in_center);

        // 2 is the number of figures that makes up a vaccinationCenter and vaccin_boxes_in_center are the number of figures representing the vaccin boxes in the center
        nrFigures += 2 + vaccin_boxes_in_center;
        id++;
    }
    id = 0;
    for (int hub_idx = 0; hub_idx < ((int) hubs.size()); hub_idx++) {

        //center x-coordinate position of the hub in 3d space
        double center_position_of_hub = width_of_hub/2.0 + space_between_hubs + hub_idx*(width_of_hub + space_between_hubs);

        //total vaccin boxes present in hub
        int vaccin_boxes_in_hub = std::min(5,(hubs[hub_idx]->getTotalvaccins() + 9999)/10000);

        nrFigures += 1 + vaccin_boxes_in_hub;
        id++;
        std::set<unsigned int> centers_connected = planned_hubs[daycount][hubs[hub_idx]];
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
