#include <string>
#include "leapfrog.hpp"

int main() {

    int N;
    double eps = pow(1000, -0.3);
    double delta = 0.8;
    double h = 0.0001;
    double time_end = .1;
    double size = 12.;
    int write = 10;
    
    string file_name = "test";

    vector<Particle> particles; 

    ifstream infile;
    infile.open("initial_plummer.txt");
    N = read(particles, infile);

    leapFrog(particles, size, eps, delta, h, time_end, N, write, file_name);


    return 0;
}