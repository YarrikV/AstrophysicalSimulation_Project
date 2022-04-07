#include <string>
#include "leapfrog.hpp"

int main() {

    unsigned int N;
    double eps = 0.;
    double delta = 0.;
    double h = 0.01;
    double time_end = 10;
    double size = 3.;
    //~ int write = 10;
    
    string file_name = "3body_stable";

    vector<Particle> particles; 

    ifstream infile;
    infile.open("init_cond_stable_3body_3d.txt");
    N = read(particles, infile);

    PEFRL(particles, size, eps, delta, h, time_end, N, file_name);


    return 0;
}
