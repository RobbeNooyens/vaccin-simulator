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
#include "json/JValue.h"
#include "json/JArray.h"
#include "entities/VaccinationCenter.h"
#include "entities/Hub.h"

#define C_ITERATE(type, iteratable, name) for(type::const_iterator name = iteratable.begin(); name != iteratable.end(); name++)
#define ITERATE(type, iteratable, name) for(type::iterator name = iteratable.begin(); name != iteratable.end(); name++)

// Constructors

Simulator::Simulator(): initCheck(this), daycount(0) {
    ENSURE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
}

bool Simulator::properlyInitialized() const {
    return initCheck == this;
}

// IO

void Simulator::importSimulation(const std::string& fileName) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been properly initialized!");
    REQUIRE(!fileName.empty(), "Filename cannot be empty!");
    JObject* json = xmlParser.parse(fileName);
    fromJSON(json);
    // TODO: delete json
}

void Simulator::exportSimulation(const std::string& fileName) const {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(!fileName.empty(), "Filename cannot be empty!");
    std::ofstream file;
    file.open(fileName.c_str());
    assert(file.is_open());
    C_ITERATE(Hubs, hubs, hub) {
        (*hub).toSummaryStream(file);
        (*hub).toProgressStream(file);
    }
    file.close();
    ENSURE(!file.is_open(), "File wasn't closed properly!");
}

void Simulator::fromJSON(JObject *json) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(json->contains("centra"), "Can't load Simulator from JSON with missing field 'centra'");
    REQUIRE(json->contains("hubs"), "Can't load Simulator from JSON with missing field 'hubs'");
    JValues centersJSON = json->getValue("centra")->asJArray()->getItems();
    ITERATE(JValues, centersJSON, center) {
        VaccinationCenter c;
        c.fromJSON((*center)->asJObject());
        centers.push_back(c);
    }
    JValues hubsJSON = json->getValue("hubs")->asJArray()->getItems();
    ITERATE(JValues, hubsJSON, hub) {
        Hub h;
        h.fromJSON((*hub)->asJObject(), centers);
        hubs.push_back(h);
    }
    ENSURE(!hubs.empty(), "Couldn't load hubs properly!");
    ENSURE(!centers.empty(), "Couldn't load centers properly!");
}

// Simulation controls

void Simulator::run(const unsigned int cycles) {
    REQUIRE(properlyInitialized(), "Simulator object hasn't been initialized properly!");
    REQUIRE(cycles != 0, "Cycles cannot be 0!");
    unsigned int lastDay = daycount + cycles, oldDaycount = daycount;
    for(; daycount < lastDay; daycount++){
        ITERATE(Hubs, hubs, hub) {
            (*hub).simulateDay(daycount);
        }
    }
    ENSURE(daycount == oldDaycount + cycles, "Simulator didn't succesfully finish the right amount of cycles!");
}
