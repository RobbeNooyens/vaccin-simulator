#include <bits/stdc++.h>

using namespace std;

void generate_animation(double hubs_aantal, double centers_aantal, int daycount) {
    double width_hub, width_center, hub_space, center_space;
    if (hubs_aantal < 4) {
        double width_hub = 0.5;
        double hub_space = (2.0 - (hubs_aantal * 0.5))/(hubs_aantal + 1.0);
    } else {
        double width_hub = 4.0/(3.0*hubs_aantal + 1.0);
        double hub_space = width_hub/2.0;
    }
    if (centers_aantal < 4) {
        double width_center = 0.5;
        double center_space = (2.0 - (centers_aantal * 0.5))/(centers_aantal + 1.0);
    } else {
        double width_center = 4.0/(3.0*centers_aantal + 1.0);
        double center_space = width_center/2.0;
    }
    //2*scale = width_hub;
    //2*scale = width_center;
    int nrFigures = (int) hubs_aantal + (int) centers_aantal + 1; //vermenigvuldig nog factoren met bv aantal objecten waaruit een vacinatiecentrum bestaat.
    // for (int i = 0; i < (int) hubs.size(); i++) {
    //     nrFigures += hubs[i]->get_connections();
    // }
    ofstream myfile; //verander de [Figure0] bij de eerste cube
    myfile.open("frame" + to_string(daycount) + ".ini"); //"animation" is hetzelfde als "ZBuffering" maar het genereert per dag meerdere fotos;
    myfile << "[GENERAL]\ntype = \"animation\"\nsize = 1024\neye = (20,15,50)\nbackgroundcolor = (0.0, 0.0, 0.0)\nnrFigures = " + to_string(nrFigures) + "\n";
    for (int i = 0; i < (int) centers_aantal; i++) {
        myfile << "\n[Figure" + to_string(i) + "]\ntype = \"Cube\"\nscale = " + to_string(width_center/2.0) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(width_center/2.0 + center_space) + ", " + to_string(width_center/2.0) + ", 0)\ncolor = (1, 0, 0)\n"; //verander color naargelang de aantal gevacineerde inwoners;
    }
    for (int i = (int) centers_aantal; i < ((int) hubs_aantal + (int) centers_aantal); i++) {
        myfile << "\n[Figure" + to_string(i) + "]\ntype = \"Cube\"\nscale = " + to_string(width_hub/2.0) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(width_hub/2.0 + hub_space) + ", " + to_string(width_hub/2.0) + ", 0)\ncolor = (0, 1, 0)\n";
    }
    myfile << "\n[Figure" + to_string(nrFigures-1) + "]\ntype = \"Cube\"\nscale = 1\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (1, 1, -1)\ncolor = (0, 0, 1)\n";
    myfile.close() //zet nog in ini file welke hub me welke centra connected is om autos te generere.
}

int main() {
    generate_animation(3, 4, 0);
}