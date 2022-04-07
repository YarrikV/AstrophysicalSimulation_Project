#ifndef VEC2D_H_
#define VEC2D_H_

#include <iostream>
#include <cmath>

using namespace std;

class Vec2d {

	double _x;
	double _y;

public:
	// Default vector is the origin
	Vec2d() { _x = 0; _y = 0; }
	Vec2d(double x, double y) { _x = x; _y = y; }

	// Functions which return the values of the coordinates
	double x() const { return _x; }
	double y() const { return _y; }

	// Functions which calculate powers of the norm
	double norm() const { return sqrt(_x * _x + _y * _y); }
	double norm2() const { return _x * _x + _y * _y; }
	double norm3() const { double r = sqrt(_x * _x + _y * _y); return r * r * r; }

	// Operator overloading
	Vec2d& operator+=(Vec2d v) {
		_x += v._x;
		_y += v._y;
		return *this;
	}
	Vec2d& operator-=(Vec2d v) {
		_x -= v._x;
		_y -= v._y;
		return *this;
	}
	Vec2d& operator*=(double s) {
		_x *= s;
		_y *= s;
		return *this;
	}
	Vec2d& operator /=(double s) {
		_x /= s;
		_y /= s;
		return *this;
	}
};

// * Define operations between two vectors
Vec2d operator+(Vec2d a, Vec2d b) { return a += b; }
Vec2d operator-(Vec2d a, Vec2d b) { return a -= b; }

// * Define operation between a vector and a scalar
Vec2d operator*(Vec2d a, double s) { return a *= s; }
Vec2d operator*(double s, Vec2d a) { return a *= s; }
Vec2d operator/(Vec2d a, double s) { return a /= s; }
Vec2d operator/(double s, Vec2d a) { return a /= s; }
// * Define a print function
void print(Vec2d a) { cout << a.x() << ' ' << a.y() << endl; }

#endif