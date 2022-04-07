#include "leapfrog.hpp"
#include <string>

int main() {

    int N = 3;
    double eps = 0.3;
    double h = 0.0001;
    double time_end = 60;
    
    string file_name = to_string(N) + "-body_eps=" + to_string(eps) + "_h=" + to_string(h);

    Particle a = Particle(Vec2d(0., 3.), Vec2d(0., 0.), 4.);
    Particle b = Particle(Vec2d(4., 0.), Vec2d(0., 0.), 3.);
    Particle c = Particle(Vec2d(0., 0.), Vec2d(0., 0.), 5.);

    vector<Particle> particles = {a, b, c}; 

    leapFrog(particles, eps, h, time_end, N, file_name);


    return 0;
}