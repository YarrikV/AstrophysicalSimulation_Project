#ifndef VEC3D_H_
#define VEC3D_H_
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

// * A basic 3D vector class

class Vec3d {

	double _x;
	double _y;
	double _z;

public:
	// * Default vector is the origin
	Vec3d() { _x = 0; _y = 0; _z = 0; }
	Vec3d(double x, double y, double z) { _x = x; _y = y; _z = z; }

	// * Functions which return the values of the coordinates
	double x() const { return _x; }
	double y() const { return _y; }
	double z() const { return _z; }

	// * Functions which calculate powers of the norm
	double norm() const { return sqrt(_x * _x + _y * _y + _z * _z); }
	double norm2() const { return _x * _x + _y * _y + _z * _z; }
	double norm3() const { double r = sqrt(_x * _x + _y * _y + _z * _z); return r * r * r; }

	// * Operator overloading
	Vec3d& operator+=(Vec3d v) {
		_x += v._x;
		_y += v._y;
		_z += v._z;
		return *this;
	}
	Vec3d& operator-=(Vec3d v) {
		_x -= v._x;
		_y -= v._y;
		_z -= v._z;
		return *this;
	}
	Vec3d& operator*=(double s) {
		_x *= s;
		_y *= s;
		_z *= s;
		return *this;
	}
	Vec3d& operator /=(double s) {
		_x /= s;
		_y /= s;
		_z /= s;
		return *this;
	}
};

// * Define operations between two vectors
Vec3d operator+(Vec3d a, Vec3d b) { return a += b; }
Vec3d operator-(Vec3d a, Vec3d b) { return a -= b; }

// * Define operation between a vector and a scalar
Vec3d operator*(Vec3d a, double s) { return a *= s; }
Vec3d operator*(double s, Vec3d a) { return a *= s; }
Vec3d operator/(Vec3d a, double s) { return a /= s; }
Vec3d operator/(double s, Vec3d a) { return a /= s; }

// * Define a print function
void print(Vec3d a) { cout << a.x() << ' ' << a.y() << ' ' << a.z() << endl; }

#endif