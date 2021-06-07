// ╒════════════════════════════════════════════╕
// │ Authors: Mohammed Shakleya, Robbe Nooyens  │
// │ Project: Vaccimulator                      │
// │ Version: 2.1                               │
// │             UAntwerpen 2021                │
// ╘════════════════════════════════════════════╛

#include <fstream>
#include <set>
#include <map>
#include "Simulator.h"
#include "../utilities/utils.h"
#include "../../DesignByContract.h"
#include "../json/JObject.h"
#include "../json/JValue.h"
#include "../json/JArray.h"
#include "../json/JKeys.h"
#include "Planning.h"
#include <algorithm>

#define C_ITERATE(type, iteratable, name) for(type::const_iterator name = iteratable.begin(); name != iteratable.end(); name++)
#define ITERATE(type, iteratable, name) for(type::iterator name = iteratable.begin(); name != iteratable.end(); name++)
#define COMMA ,

#define IMAGE_SIZE 1024
#define SPACING 3

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

void Simulator::run(const unsigned int cycles, bool smartDistribution) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(cycles != 0, "Cycles cannot be 0!");
    REQUIRE(isConsistent(), "Simulation needs to be consistent to run!");
    unsigned int lastDay = daycount + cycles, oldDaycount = daycount;
    while(daycount < lastDay){
        // Clear transportations
        statistics.getTransportations().clear();
        // Deliver vaccines to the hub if expected and transport vaccines to the centers
        ITERATE(std::vector<Hub*>, hubs, hub)(*hub)->simulateDay(daycount, statistics, smartDistribution);
        // Vaccinate inhabitants (should happen here to prevent double vaccinations)
        ITERATE(VaccinationCenters, centers, center)(*center)->vaccinateInhabitants(daycount, statistics);
        if(statisticsOutputStream)
            statistics.printStatistics(*statisticsOutputStream);
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
    statistics.reset();
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

void Simulator::exportSimulationIniFile(std::string &output, unsigned int frames) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(frames >= 1, "Frames should be higher than 1!");
    std::ofstream file;
    std::map<Hub*, std::vector<VaccinationCenter*> > transportations = statistics.getTransportations();

    unsigned int transportationsCount = 0;
    ITERATE(std::map<Hub* COMMA std::vector<VaccinationCenter*> >, transportations, transport) {
        transportationsCount += transport->second.size();
    }

    for(int frame = 1; frame <= (int) frames; frame++) {
        std::stringstream fileName;
        fileName << output << frame << ".ini";
        file.open(fileName.str().c_str());

        file << "[General]" << std::endl;
        file << "size = " << IMAGE_SIZE << std::endl;
        file << "backgroundcolor = " << "(0,0,0)" << std::endl;
        file << "type = \"" << "ZBuffering" << "\"" << std::endl;
        file << "eye = " << "(50, 50, 50)" << std::endl;
        file << "nrFigures = " << centers.size() + hubs.size() + transportationsCount << std::endl;
        file << std::endl;

        int index = 0;
        unsigned int totalInhabitants = 0;
        for(int i = 0; i < (int) centers.size(); i++, index++) {
            file << "[Figure" << index << "]" << std::endl;
            file << "type = \"" << "Cube" << "\"" << std::endl;
            file << "scale = 1" << std::endl;
            file << "rotateX = 0" << std::endl;
            file << "rotateY = 0" << std::endl;
            file << "rotateZ = 0" << std::endl;
            file << "center = (" << 0 << ", " << i * SPACING << ", " << 0 << ")" << std::endl;
            double intensity = centers[i]->getPercentageVaccines();
            file << "color = (" << intensity << ", " << intensity << ", " << 50 << ")" << std::endl;
            file << std::endl;
            totalInhabitants += centers[i]->getInhabitants();
        }
        for(int i = 0; i < (int) hubs.size(); i++, index++) {
            file << "[Figure" << index << "]" << std::endl;
            file << "type = \"" << "Cylinder" << "\"" << std::endl;
            file << "height = " << 2 << std::endl;
            file << "n = " << 360 << std::endl;
            file << "scale = 1" << std::endl;
            file << "rotateX = 0" << std::endl;
            file << "rotateY = 0" << std::endl;
            file << "rotateZ = 0" << std::endl;
            file << "center = (" << 8 * SPACING << ", " << i * SPACING << ", " << 0 << ")" << std::endl;
            double intensity = std::min(1.0, (double) hubs[i]->getTotalVaccinesCount() / (double) totalInhabitants);
            file << "color = (" << intensity << ", " << intensity << ", " << 50 << ")" << std::endl;
            file << std::endl;
        }
        for(int h = 0; h < (int) hubs.size(); h++) {
            Hub* hub = hubs[h];
            if(transportations.find(hub) == transportations.end())
                continue;
            for(int c = 0; c < (int) centers.size(); c++) {
                VaccinationCenter* center = centers[c];
                std::vector<VaccinationCenter*> transportCenters = transportations[hub];
                if(std::find(transportCenters.begin(), transportCenters.end(), center) == transportCenters.end())
                    continue;
                double xFrom = 8 * SPACING;
                double yFrom = h * SPACING;
                double xTo = 0;
                double yTo = c * SPACING;
                // to - from
                double x = xFrom + frame * ((xTo - xFrom) / frames);
                double y = yFrom + frame * ((yTo - yFrom) / frames);

                file << "[Figure" << index << "]" << std::endl;
                file << "type = \"" << "Sphere" << "\"" << std::endl;
                file << "n = " << 5 << std::endl;
                file << "scale = 1" << std::endl;
                file << "rotateX = 0" << std::endl;
                file << "rotateY = 0" << std::endl;
                file << "rotateZ = 0" << std::endl;
                file << "center = (" << x << ", " << y << ", " << 0 << ")" << std::endl;
                file << "color = (" << 0 << ", " << 0 << ", " << 50 << ")" << std::endl;
                file << std::endl;

                index++;
            }
        }
        file.close();
    }
}

SimulationData &Simulator::getStatistics() {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    return statistics;
}
