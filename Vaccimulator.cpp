// ╒============================================╕
// | Authors: Mohammed Shakleya, Robbe Nooyens  |
// | Project: Vaccimulator                      |
// | Version: 1.0                               |
// |             UAntwerpen 2021                |
// ╘============================================╛

#include <iostream>
#include <string>

#include "Simulator.h"

#include "json/JObject.h"
#include "json/JArray.h"
#include "json/JValue.h"
#include "utils.h"

int main() {
    JObject* json = new JObject();
    // Initialize Hub
    JObject* h = new JObject();
    json->insertValue("hub", new JValue(h));
    h->insertValue("levering", new JValue((unsigned int) 100));
    h->insertValue("interval", new JValue((unsigned int) 6));
    h->insertValue("transport", new JValue((unsigned int) 40));
    // Initialize centra
    JArray* centra = new JArray();
    json->insertValue("centra", new JValue(centra));

    JObject* center1 = new JObject();
    center1->insertValue("adres", new JValue((std::string) "Mainstreet"));
    center1->insertValue("capaciteit", new JValue((unsigned int) 200));
    center1->insertValue("inwoners", new JValue((unsigned int) 5043));
    center1->insertValue("naam", new JValue((std::string) "Center 1"));
    centra->insertValue(new JValue(center1));

    JObject* center2 = new JObject();
    center2->insertValue("adres", new JValue((std::string) "Wallstreet"));
    center2->insertValue("capaciteit", new JValue((unsigned int) 600));
    center2->insertValue("inwoners", new JValue((unsigned int) 8011));
    center2->insertValue("naam", new JValue((std::string) "Center 2"));
    centra->insertValue(new JValue(center2));

    JObject* center3 = new JObject();
    center3->insertValue("adres", new JValue((std::string) "Route 66"));
    center3->insertValue("capaciteit", new JValue((unsigned int) 100));
    center3->insertValue("inwoners", new JValue((unsigned int) 3021));
    center3->insertValue("naam", new JValue((std::string) "Center 3"));
    centra->insertValue(new JValue(center3));

    Hub hub = Hub();
    hub.fromJSON(json);

    std::ofstream file;
    file.open("tests/presentation/out/hub_output.txt");
    hub.toStream(file);
    hub.toStream(std::cout);
    file.close();
    bool equal = FileUtil::FileCompare("tests/presentation/out/hub_output.txt", "tests/presentation/expected/hub_output.txt");
    std::cout << equal;
    delete json;
//    Simulator simulator = Simulator();
//    simulator.importSimulation("io/input.xml");
//    simulator.run(1);
//    simulator.exportSimulation("io/output.txt");
    return 0;
}
