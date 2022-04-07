#ifndef VEC3D_H_
#define VEC3D_H_
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

// * File containing the 3D vector and tensor class

class Vec3d {

	double _x;
	double _y;
	double _z;

public:
	// Default vector is the origin
	Vec3d() { _x = 0; _y = 0; _z = 0; }
	Vec3d(double x, double y, double z) { _x = x; _y = y; _z = z; }

	// Functions which return the values of the coordinates
	double x() const { return _x; }
	double y() const { return _y; }
	double z() const { return _z; }

	// Functions which calculate powers of the norm
	double norm() const { return sqrt(_x * _x + _y * _y + _z * _z); }
	double norm2() const { return _x * _x + _y * _y + _z * _z; }
	double norm3() const { double r = sqrt(_x * _x + _y * _y + _z * _z); return r * r * r; }

	// Operator overloading
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

class Tensor {
private:

	double tensor[3][3];

public:
	// * Empty tensor
	Tensor() {
		for(unsigned int i = 0; i < 3; i++) {
			for(unsigned int j = 0; j < 3; j++) {
				this->tensor[i][j] = 0.;
			}
		}
	}

	// * Tensor product of the same vectores
	Tensor(Vec3d a) {
		this->tensor[0][0] = 3 * a.x() * a.x() - a.norm2();
		this->tensor[0][1] = 3 * a.x() * a.y();
		this->tensor[0][2] = 3 * a.x() * a.z();
		this->tensor[1][0] = this->tensor[0][1];
		this->tensor[1][1] = 3 * a.y() * a.y() - a.norm2();
		this->tensor[1][2] = 3 * a.y() * a.z();
		this->tensor[2][0] = this->tensor[0][2];
		this->tensor[2][1] = this->tensor[1][2];
		this->tensor[2][2] = 3 * a.z() * a.z() - a.norm2();

	}

public:
	// * Identity matrix
	void identity() {
		for(unsigned int i = 0; i < 3; i++) {
			this->tensor[i][i] = 1.;
		}
	}

	// * Setters and getters
	void set(unsigned int i, unsigned int j, double x) { this->tensor[i][j] = x; }
	double get(unsigned int i, unsigned int j) { return this->tensor[i][j]; }

	// * Operator overloading
	Tensor& operator+=(Tensor v) {
		for(unsigned int i = 0; i < 3; i++) {
			for(unsigned int j = 0; j < 3; j++) {
				this->tensor[i][j] += v.tensor[i][j];
			}	
		}
		return *this;
	}

	Tensor& operator-=(Tensor v) {
		for(unsigned int i = 0; i < 3; i++) {
			for(unsigned int j = 0; j < 3; j++) {
				this->tensor[i][j] -= v.tensor[i][j];
			}	
		}
		return *this;
	}

	Tensor& operator*=(double s) {
		for(unsigned int i = 0; i < 3; i++) {
			for(unsigned int j = 0; j < 3; j++) {
				this->tensor[i][j] *= s;
			}	
		}
		return *this;
	}

	Tensor& operator/=(double s) {
		for(unsigned int i = 0; i < 3; i++) {
			for(unsigned int j = 0; j < 3; j++) {
				this->tensor[i][j] /= s;
			}	
		}
		return *this;
	}


};

// * Define operations between two tensors
Tensor operator+(Tensor a, Tensor b) { return a += b; }
Tensor operator-(Tensor a, Tensor b) { return a -= b; }

// * Define operation between a tensor and a scalar
Tensor operator*(Tensor a, double s) { return a *= s; }
Tensor operator*(double s, Tensor a) { return a *= s; }
Tensor operator/(Tensor a, double s) { return a /= s; }
Tensor operator/(double s, Tensor a) { return a /= s; }

// * Define vector * vector
double operator*(Vec3d a, Vec3d b) {
	return a.x()*b.x() + a.y() * b.y() + a.z() * b.z();
}

// * Define tensor * vector
Vec3d operator*(Tensor a, Vec3d v) {

	double x = a.get(0, 0) * v.x() + a.get(0, 1) * v.y() + a.get(0, 2) + v.z();
	double y = a.get(1, 0) * v.x() + a.get(1, 1) * v.y() + a.get(1, 2) + v.z();
	double z = a.get(2, 0) * v.x() + a.get(2, 1) * v.y() + a.get(2, 2) + v.z();

	return Vec3d(x, y, z);
}

// * Define a Kronecker-Delta function
double Kronecker(unsigned int i, unsigned int j) {
	if(i == j) {
		return 1.;
	}
}


#endif