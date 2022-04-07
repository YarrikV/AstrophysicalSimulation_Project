#include <string>
#include "leapfrog.hpp"

int main() {

    int N;
    double eps = 0.3;
    double delta = 0.25;
    double h = 0.01;
    double time_end = 60;
    
    // * string file_name = 'to_string(N) + "-body_eps=" + to_string(eps) + "_h=" + to_string(h)';
    string file_name = "SSOct";

    vector<Particle> particles; 

    ifstream infile;
    infile.open("initial_plummer.txt");
    N = read(particles, infile);

    leapFrog(particles, eps, delta, h, time_end, N, file_name);

    return 0;
}
