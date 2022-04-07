#ifndef LEAPFROG_H_
#define LEAPFROG_H_

#include <fstream>
#include <iomanip>
#include <vector>
#include "Particle.hpp"

// * Energy calculating the total energy of the system
double energy(const vector<Particle>& particles, double N, double eps) {
    double energy = 0;
    double kinetic_energy;
    double potential_energy;

    for(unsigned int i = 0; i < N; i++) {
        Particle elem = particles[i];
        kinetic_energy = elem.m() * elem.v().norm2() * 0.5;
        potential_energy = 0;
        
        for(unsigned int j = 0; j < i; j++) {
            if(j != i) {
                Vec2d relative_vector = elem.r() - particles[j].r();
                potential_energy -= particles[j].m() / hypot(eps, relative_vector.norm());
            }
        }

        energy += (kinetic_energy + potential_energy);
    }

    return energy;
}

// * Give the acceleration acting on each particle given the particles
vector<Vec2d> acceleration2d(const vector<Particle>& particles, double N, double eps) {

	vector<Vec2d> acc;
    Vec2d a_i; // * Total acceleration on i

	for (unsigned int i = 0; i < N; i++) {
        Particle elem = particles[i];
        a_i = Vec2d();
        for(unsigned int j = 0; j < N; j++) {
            if(j != i) {
                Vec2d relative_vector = elem.r() - particles[j].r();
                a_i -= (particles[j].m() * relative_vector) / pow( hypot(eps, relative_vector.norm()), 3 );
            }
        }

		acc.push_back(a_i);
	}
	return acc;
}

void writeOrbit(const vector<Particle>& particles, double N, ofstream& file) {
    for(unsigned int i = 0; i < N; i++) {
        file << particles[i].r().x() << ' ' 
        << particles[i].r().y() << ' '; 
    }
    file << endl;
}


void leapFrog(vector<Particle>& particles, double eps, double h, double time_end, double N, string file_name) {

    ofstream outfile_orbit("Orbit_" + file_name + ".txt");
    ofstream outfile_energy("Energy_" + file_name + ".txt");
    outfile_orbit << setprecision(12);
    outfile_energy << setprecision(12);

    double energy_0 = energy(particles, N, eps);

    writeOrbit(particles, N, outfile_orbit);

    vector<Particle> r_half;
    vector<Vec2d> acc = acceleration2d(particles, N, eps);

    for(unsigned int i = 0; i < N; i++) {
        r_half.push_back( Particle(particles[i].r() + h * 0.5 * particles[i].v() + h * h * 0.125 * acc[i],
        Vec2d(), particles[i].m()));
    }

    Vec2d r_next;
    double error;

    for (double t = h; t <= time_end; t += h) {

        vector<Vec2d> acc = acceleration2d(r_half, N, eps);

        for(unsigned int i = 0; i < N; i++) {
            particles[i].addVelocity(h * acc[i]);
            r_next = r_half[i].r() + h * particles[i].v();
            particles[i].setPosition((r_next + r_half[i].r()) * 0.5);
            r_half[i].setPosition(r_next);
        }

        writeOrbit(particles, N, outfile_orbit);

        error = abs((energy(particles, N, eps) - energy_0) / energy_0);
        outfile_energy << t << ' ' << error << endl;
    }

    outfile_orbit.close();
    outfile_energy.close();

}

#endif