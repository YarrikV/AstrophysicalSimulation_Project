#include "leapfrog.hpp"
#include <string>

int main() {

    int N = 2;
    double eps = 0.3;
    double h = 0.01;
    double time_end = 60;
    
    string file_name = to_string(N) + "-body_eps=" + to_string(eps) + "_h=" + to_string(h);

    Particle a = Particle(Vec3d(0.1, 0., 0.), Vec3d(0., 0.003, 0.), .005);
    Particle b = Particle(Vec3d(-0.1, 0., 0.), Vec3d(0., -0.003, 0.), .005);
    // Particle c = Particle(Vec3d(0., 0., 0.), Vec3d(0., 0., 0.), 5.);

    vector<Particle> particles = {a, b}; 

    leapFrog(particles, eps, h, time_end, N, file_name);


    return 0;
}