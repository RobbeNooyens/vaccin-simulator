#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <cmath>
#include <ostream>
#include <fstream>

using namespace std;

void generate_animation(double hubs_aantal, double centers_aantal, int daycount, vector<int> hubs) {
    double width_hub, width_center, hub_space, center_space;
    long long id = 0;
    if ((int) hubs_aantal < 4) {
        width_hub = 0.5;
        hub_space = (2.0 - (hubs_aantal * 0.5))/(hubs_aantal + 1.0);
    } else {
        width_hub = 4.0/(3.0*(hubs_aantal) + 1.0);
        hub_space = width_hub/2.0;
    }
    if ((int) centers_aantal < 4) {
        width_center = 0.5;
        center_space = (2.0 - ((centers_aantal) * 0.5))/((centers_aantal) + 1.0);
    } else {
        width_center = 4.0/(3.0*(centers_aantal) + 1.0);
        center_space = width_center/2.0;
    }
    int nrFigures = 0;
    ofstream myfile("frame" + to_string(daycount) + ".ini");
    //"animation" is hetzelfde als "ZBuffering" maar het genereert per dag meerdere fotos;
    for (int i = 0; i < (int) centers_aantal; i++) {
        myfile << "\n[Figure" + to_string(nrFigures) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_center/2.0) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(width_center/2.0 + center_space + i*(width_center + center_space)) + ", " + to_string(2.0 - width_center/2.0) + ", " + to_string(width_center/2.0 + 0.1*width_center) + ")\ncolor = (1, 0, 0)\nobject = vaccinationcenter\n";
        myfile << "\n[Figure" + to_string(nrFigures + 1) + "]\ntype = \"Cone\"\nid = " + to_string(id) + "\nscale = " + to_string(width_center/2.0) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(width_center/2.0 + center_space + i*(width_center + center_space)) + ", " + to_string(2.0 - width_center/2.0) + ", " + to_string(width_center + 0.1*width_center) + ")\ncolor = (0, 1, 0)\nn = 20\nheight = 10.0\nobject = vaccinationcenter\n";
        myfile << "\n[Figure" + to_string(nrFigures + 2) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_center/10) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(width_center/2.0 + center_space + i*(width_center + center_space) + width_center/10) + ", " + to_string(2.0 - width_center/10) + ", " + to_string(width_center/10) + ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
        myfile << "\n[Figure" + to_string(nrFigures + 3) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_center/10) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(width_center/2.0 + center_space + i*(width_center + center_space) + width_center/10) + ", " + to_string(2.0 - width_center/5 - width_center/10) + ", " + to_string(width_center/10) + ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
        myfile << "\n[Figure" + to_string(nrFigures + 4) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_center/10) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(width_center/2.0 + center_space + i*(width_center + center_space) - width_center/10) + ", " + to_string(2.0 - width_center/10) + ", " + to_string(width_center/10) + ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
        myfile << "\n[Figure" + to_string(nrFigures + 5) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_center/10) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(width_center/2.0 + center_space + i*(width_center + center_space) - width_center/10) + ", " + to_string(2.0 - width_center/5 - width_center/10) + ", " + to_string(width_center/10) + ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
        myfile << "\n[Figure" + to_string(nrFigures + 6) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_center/10) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(width_center/2.0 + center_space + i*(width_center + center_space)) + ", " + to_string(2.0 - width_center/5) + ", " + to_string(width_center/5 + width_center/10) + ")\ncolor = (0, 0, 1)\nobject = vaccinationcenter_box\n";
        nrFigures += 7;
        id++;
    }
    id = 0;
    for (int i = 0; i < ((int) hubs_aantal); i++) {
        double center_position = width_hub/2.0 + hub_space + i*(width_hub + hub_space);
        myfile << "\n[Figure" + to_string(nrFigures) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_hub/2.0) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position) + ", " + to_string(width_hub/2.0) + ", " + to_string(width_hub/2.0 + 0.1*width_hub) + ")\ncolor = (0, 1, 0)\nobject = hub\n";
        myfile << "\n[Figure" + to_string(nrFigures + 1) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_hub/10.0) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position + width_hub/10.0) + ", " + to_string(width_hub/10.0) + ", " + to_string(width_hub/10.0) + ")\ncolor = (0, 0, 1)\nobject = hub\n";
        myfile << "\n[Figure" + to_string(nrFigures + 2) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_hub/10.0) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position + width_hub/10.0) + ", " + to_string(width_hub/5 + width_hub/10.0) + ", " + to_string(width_hub/10.0) + ")\ncolor = (0, 0, 1)\nobject = hub\n";
        myfile << "\n[Figure" + to_string(nrFigures + 3) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_hub/10.0) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position - width_hub/10.0) + ", " + to_string(width_hub/10.0) + ", " + to_string(width_hub/10.0) + ")\ncolor = (0, 0, 1)\nobject = hub\n";
        myfile << "\n[Figure" + to_string(nrFigures + 4) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_hub/10.0) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position - width_hub/10.0) + ", " + to_string(width_hub/5 + width_hub/10.0) + ", " + to_string(width_hub/10.0) + ")\ncolor = (0, 0, 1)\nobject = hub\n";
        myfile << "\n[Figure" + to_string(nrFigures + 5) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(width_hub/10.0) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position) + ", " + to_string(width_hub/5) + ", " + to_string(width_hub/5 + width_hub/10) + ")\ncolor = (0, 0, 1)\nobject = hub\n";
        nrFigures += 6;
        id++;
        for (int k = 0; k < 1; k++) {
            double car_width = std::min(width_hub/2.0, width_center/2.0);
            double wheel_width = std::min(width_hub/2.0, width_center/2.0)/4;
            myfile << "\n[Figure" + to_string(nrFigures) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(car_width/2) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position) + ", " + to_string(width_hub/2.0) + ", " + to_string(car_width/2.0 + wheel_width) + ")\ncolor = (1, 0.0784, 0.5764)\nobject = car\nvisit = " + to_string(i) + "\n"; //+ 0.1*wheel_width
            myfile << "\n[Figure" + to_string(nrFigures+1) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(car_width/6) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position) + ", " + to_string(width_hub/2.0) + ", " + to_string(car_width/6 + car_width/3 + wheel_width) + ")\ncolor = (1, 0.5, 0.2)\nobject = car_box\nvisit = " + to_string(i) + "\n";
            myfile << "\n[Figure" + to_string(nrFigures+2) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(car_width/6) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position + car_width/6) + ", " + to_string(width_hub/2.0 + car_width/6) + ", " + to_string(car_width/6 + wheel_width) + ")\ncolor = (1, 0.5, 0.2)\nobject = car_box\nvisit = " + to_string(i) + "\n";
            myfile << "\n[Figure" + to_string(nrFigures+3) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(car_width/6) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position + car_width/6) + ", " + to_string(width_hub/2.0 - car_width/6) + ", " + to_string(car_width/6 + wheel_width) + ")\ncolor = (1, 0.5, 0.2)\nobject = car_box\nvisit = " + to_string(i) + "\n";
            myfile << "\n[Figure" + to_string(nrFigures+4) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(car_width/6) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position - car_width/6) + ", " + to_string(width_hub/2.0 + car_width/6) + ", " + to_string(car_width/6 + wheel_width) + ")\ncolor = (1, 0.5, 0.2)\nobject = car_box\nvisit = " + to_string(i) + "\n";
            myfile << "\n[Figure" + to_string(nrFigures+5) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = " + to_string(car_width/6) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position - car_width/6) + ", " + to_string(width_hub/2.0 - car_width/6) + ", " + to_string(car_width/6 + wheel_width) + ")\ncolor = (1, 0.5, 0.2)\nobject = car_box\nvisit = " + to_string(i) + "\n";

            myfile << "\n[Figure" + to_string(nrFigures+6) + "]\ntype = \"Sphere\"\nid = " + to_string(id) + "\nscale = " + to_string(wheel_width/2) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position + car_width/2 - wheel_width/2) + ", " + to_string(width_hub/2.0 + car_width/2 - wheel_width) + ", " + to_string(wheel_width/2) + ")\ncolor = (1, 1, 1)\nn = 5\nobject = car\nvisit = " + to_string(i) + "\n";
            myfile << "\n[Figure" + to_string(nrFigures+7) + "]\ntype = \"Sphere\"\nid = " + to_string(id) + "\nscale = " + to_string(wheel_width/2) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position + car_width/2 - wheel_width/2) + ", " + to_string(width_hub/2.0 - car_width/2 + wheel_width) + ", " + to_string(wheel_width/2) + ")\ncolor = (1, 1, 1)\nn = 5\nobject = car\nvisit = " + to_string(i) + "\n";
            myfile << "\n[Figure" + to_string(nrFigures+8) + "]\ntype = \"Sphere\"\nid = " + to_string(id) + "\nscale = " + to_string(wheel_width/2) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position - car_width/2 + wheel_width/2) + ", " + to_string(width_hub/2.0 + car_width/2 - wheel_width) + ", " + to_string(wheel_width/2) + ")\ncolor = (1, 1, 1)\nn = 5\nobject = car\nvisit = " + to_string(i) + "\n";
            myfile << "\n[Figure" + to_string(nrFigures+9) + "]\ntype = \"Sphere\"\nid = " + to_string(id) + "\nscale = " + to_string(wheel_width/2) + "\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (" + to_string(center_position - car_width/2 + wheel_width/2) + ", " + to_string(width_hub/2.0 - car_width/2 + wheel_width) + ", " + to_string(wheel_width/2) + ")\ncolor = (1, 1, 1)\nn = 5\nobject = car\nvisit = " + to_string(i) + "\n";
            nrFigures += 10;
            id++;
        }
    }
    myfile << "\n[Figure" + to_string(nrFigures) + "]\ntype = \"Cube\"\nid = " + to_string(id) + "\nscale = 1\nrotateX = 0\nrotateY = 0\nrotateZ = 0\ncenter = (1, 1, -1)\ncolor = (0, 0, 1)\nobject = ground\n";
    myfile << "[General]\ntype = \"ZBufferedWireframe\"\nsize = 1024\neye = (20, 15, 50)\nbackgroundcolor = (0.0, 0.0, 0.0)\nnrFigures = " + to_string(nrFigures+1) + "\nnrVaccinationCenters = " + to_string((int) centers_aantal) + "\n";
    myfile.close();
}

int main() {
    vector<int> connect = {1, 1, 1, 1, 1};
    generate_animation(5, 5, 0, connect);
}
