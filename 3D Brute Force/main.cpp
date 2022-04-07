#include "leapfrog.hpp"
#include <string>

int main() {

    int N;
    double eps = 0.03;
    double h = 0.01;
    double time_end = 1000;
    
    string file_name = to_string(N) + "-body_eps=" + to_string(eps) + "_h=" + to_string(h);
    
    vector<Particle> particles;
    string initCondFile = "..\\Data\\init_cond_ss.txt";
    ifstream initCond(initCondFile);
    N = read(particles, initCond);
    
    leapFrog(particles, eps, h, time_end, N, file_name);


    return 0;
}
