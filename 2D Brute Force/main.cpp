#include "leapfrog.hpp"

int main() {

    // int N;
    int N = 3;
    double eps = 0.51;
    double h = 0.01;
    double time_end = 100;
    string file_name = "burrau";

    Particle a = Particle(Vec2d(0.0, 0.66667), Vec2d(), 0.005);
    Particle b = Particle(Vec2d(0.57735, -0.33333), Vec2d(), 0.005);
    Particle c = Particle(Vec2d(-0.57735, -0.33333), Vec2d(), 0.005);
    

    vector<Particle> particles = {a, b, c};

    // vector<Particle> particles;
    // string initCondFile = "..\\Data\\init_cond_ss.txt";
    // ifstream initCond(initCondFile);
    // N = read(particles, initCond);

    leapFrog(particles, eps, h, time_end, N, file_name);


    return 0;
}
