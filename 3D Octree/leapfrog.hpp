#ifndef LEAPFROG_H_
#define LEAPFROG_H_

#include <fstream>
#include <iomanip>
#include <vector>
#include "Node.hpp"

// * Energy calculating the total energy of the system
double energy(const vector<Particle>& particles, double N, double eps) {
    double energy = 0;
    double kinetic_energy;
    double potential_energy;
    Particle elem;
    Vec3d relative_vector;

    for(unsigned int i = 0; i < N; i++) {
        elem = particles[i];
        kinetic_energy = elem.m() * elem.v().norm2() * 0.5;
        potential_energy = 0;
        
        for(unsigned int j = 0; j < i; j++) {
            if(j != i) {
                relative_vector = elem.r() - particles[j].r();
                potential_energy -= elem.m() * particles[j].m() / hypot(eps, relative_vector.norm());
            }
        }

        energy += (kinetic_energy + potential_energy);
    }

    return energy;
}

void writeOrbit(const vector<Particle>& particles, double N, ofstream& file) {
    for(unsigned int i = 0; i < N; i++) {
        file << particles[i].r().x() << ' ' 
             << particles[i].r().y() << ' ' 
             << particles[i].r().z() << ' ';
        
    }
    file << endl;
}

void writeError(const vector<Particle>& particles, double energy_0, double t, double N, double eps, ofstream& file) {
    double error;
    error = abs((energy(particles, N, eps) - energy_0) / energy_0);
    file << t << ' ' << error << ' ' <<  endl;
}


void leapFrog(vector<Particle>& particles, double eps, double delta, double h, double time_end, double N, string file_name) {

    ofstream outfile_orbit(file_name + "_Orbit.txt");
    ofstream outfile_energy(file_name + "_EnergyError.txt");
    outfile_orbit << setprecision(12);
    outfile_energy << setprecision(12);

    cout << "Initialising simulation..." << endl;

    double energy_0 = energy(particles, N, eps);

    writeOrbit(particles, N, outfile_orbit);

    Node* root = new Node();
    root->buildTree(particles);

    vector<Particle> r_half;
    Vec3d acc = Vec3d();

    for(Particle elem : particles) {
        acc = root->totalForce(elem, eps, delta);
        r_half.push_back(Particle(elem.r() + h * 0.5 * elem.v() + h * h * 0.125 * acc,
        Vec3d(), elem.m()));
    }

    delete root;

    Vec3d r_next;
    double t;

    for (unsigned int n = 1; n <= time_end / h; n++) {

        t = n * h;
        root = new Node();
        root->buildTree(r_half);

        for(unsigned int i = 0; i < N; i++) {
            acc = root->totalForce(r_half[i], eps, delta);
            particles[i].addVelocity(h * acc);
            r_next = r_half[i].r() + h * particles[i].v();
            particles[i].setPosition((r_next + r_half[i].r()) * 0.5);
            r_half[i].setPosition(r_next);
        }

        delete root;

        writeOrbit(particles, N, outfile_orbit);

        if(!(n % 100)) {      
            writeError(particles, energy_0, t, N, eps, outfile_energy);
            cout << t << endl;
        }
    }

    outfile_orbit.close();
    outfile_energy.close();

    cout << endl << "Simulation ended." << endl;

}

#endif
