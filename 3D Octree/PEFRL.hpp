#ifndef PEFRL_H_
#define PEFRL_H_

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


void integrate(vector<Particle>& particles, double eps, double delta, double h, double time_end, double N, string file_name) {

    ofstream outfile_orbit(file_name + "_PEFRL_Orbit.txt");
    ofstream outfile_energy(file_name + "_PEFRL_EnergyError.txt");
    outfile_orbit << setprecision(12);
    outfile_energy << setprecision(12);

    // PEFRL Constants:
    const double ksi = 0.1786178958448091;
    const double lam = -0.2123418310626054;
    const double khi = -0.06626458266981849;

    cout << "Initialising simulation (PEFRL)..." << endl;

    double energy_0 = energy(particles, N, eps);

    writeOrbit(particles, N, outfile_orbit);

    Node* root = new Node();
    root->buildTree(particles);

    vector<Particle> r_half;
    Vec3d acc = Vec3d();

    delete root;
    Particle elem;
    Vec3d r_next;
    double t;

    for (unsigned int n = 1; n <= time_end / h; n++) {

        t = n * h;
        root = new Node();
        root->buildTree(particles);

        // step 1
        for(unsigned int i = 0; i < N; i++) {
            acc = root->totalForce(particles[i], eps, delta);
            particles[i].addPosition(ksi * h * particles[i].v());
        }
        
        // New Force calc.
        delete root;
        root = new Node();
        root->buildTree(particles);

        // step 2
        for (unsigned int i = 0; i < N; i++) {
            acc = root->totalForce(particles[i], eps, delta);
            particles[i].addVelocity(0.5 * (1 - 2*lam) * h * acc);
            particles[i].addPosition(khi * h * particles[i].v());
        }

        // New Force calc.
        delete root;
        root = new Node();
        root->buildTree(particles);

        // step 3
        for (unsigned int i = 0; i < N; i++) {
            acc = root->totalForce(particles[i], eps, delta);
            particles[i].addVelocity(lam * h * acc);
            particles[i].addPosition((1 - 2 * (khi + ksi)) * h * particles[i].v());
        }

        // New Force calc.
        delete root;
        root = new Node();
        root->buildTree(particles);

        // step 4
        for (unsigned int i = 0; i < N; i++) {
            acc = root->totalForce(particles[i], eps, delta);
            particles[i].addVelocity(lam * h * acc);
            particles[i].addPosition(khi * h * particles[i].v());
        }

        // New Force calc.
        delete root;
        root = new Node();
        root->buildTree(particles);

        // step 5
        for (unsigned int i = 0; i < N; i++) {
            acc = root->totalForce(particles[i], eps, delta);
            particles[i].addVelocity(0.5 * (1 - 2 * lam) * h * acc);
            particles[i].addPosition(ksi * h * particles[i].v());
        }

        writeOrbit(particles, N, outfile_orbit);

        if(!(n % 100)) {      
            writeError(particles, energy_0, t, N, eps, outfile_energy);
            cout << '\r' << t << flush;
        }
    }

    outfile_orbit.close();
    outfile_energy.close();

    cout << endl << "Simulation ended." << endl;

}

#endif
