#include <string>
#include "leapfrog.hpp"
#include "initialisation.hpp"

int main() {

    // * Some default values for quick running
    unsigned int N = 1000;
    double eps = pow(N, -.3);
    double delta = 1.2;
    double h = 0.01;
    double time_end = 10.;
    double size = 24.;
    int write = 10;     // * Per how many time steps we write to the files
    string initial = "initial.txt";
    string integrator = "leapfrog";
    string extension = "Nbody_";


    // * Interface
    integrator = initialiser(delta, eps, h, time_end, size, write, initial, extension);


    // * Read-in of data
    vector<Particle> particles; 

    ifstream infile;
    infile.open(initial);
    N = read(particles, infile);

    // * Choose the integrator
    if(integrator == "leapfrog") {
        leapFrog(particles, size, eps, delta, h, time_end, N, write, extension);
    }
    else if(integrator == "PEFRL") {
        PEFRL(particles, size, eps, delta, h, time_end, N, write, extension);
    }

    return 0;
}
