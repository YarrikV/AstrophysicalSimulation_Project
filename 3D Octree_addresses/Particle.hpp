#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <iostream>
#include <cmath>
#include <fstream>
#include "Vec3d.hpp"

using namespace std;

// * Define a particle class which consists of an array of 3 elements: position, velocity and mass

class Particle {

private:
    Vec3d position;
    Vec3d velocity;
    double mass;

public:
    // * Constructor initialising the particle
    Particle() {
        this->position = Vec3d();
        this->velocity = Vec3d();
        this->mass = 0;
    };

    Particle(Vec3d x, Vec3d v, double m) {
        this->position = x;
        this->velocity = v;
        this->mass = m;
    };

public:
    // * Defining some getters to extract data
    Vec3d r() const { return position; };
    Vec3d v() const { return velocity; };
    double m() const { return mass; };
    
    // * Defining some setters to change data
    void setPosition( Vec3d x ) {this->position = x; };
    void setVelocity( Vec3d v ) {this->velocity = v; };

    // * Adding a position or velocity to the one already existing
    void addPosition( Vec3d x ) {this->position += x; };
    void addVelocity( Vec3d v ) {this->velocity += v; };

};

// * reads in the particle positions, velocities and masses from a given file 'infile'
int64_t read(vector<Particle>& p, ifstream& infile){
	int n;
	double m;
	double x;
	double y;
	double z;
	double vx;
	double vy;
	double vz;
	infile >> n;
	for(int i=0;i<n;i++){
		infile >> m >> x >> y >> z
			   >> vx >> vy >> vz;
		p.push_back(Particle(Vec3d(x, y, z),Vec3d(vx, vy, vz),m));
	}
    return n;
}

#endif