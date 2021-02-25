//
// Created by robbe on 25/02/2021.
//

#ifndef VACCIN_SIMULATOR_HUB_H
#define VACCIN_SIMULATOR_HUB_H

#include <vector>
#include <istream>
#include <ostream>

class VaccinatieCentrum;
class Hub {
public:
    Hub();

    // IO Mapping
    void fromStream(std::istream inputStream);
    void toStream(std::ostream outputStream) const;

private:
    unsigned int levering;
    unsigned int interval;
    unsigned int transport;
    std::vector<VaccinatieCentrum> centra;
};


#endif //VACCIN_SIMULATOR_HUB_H
