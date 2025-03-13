#ifndef QUATERNION_H
#define QUATERNION_H

#include "vec3.h"

struct Quaternion 
{
    double w, i, j, k;

    Quaternion(double w, double i, double j, double k) : w(w), i(i), j(j), k(k) {}

    // Operators
    Quaternion operator*(const double scalar) const;
    Quaternion operator/(const double scalar) const;
	Quaternion operator+(const Quaternion& q) const;
	Quaternion operator-(const Quaternion& q) const;
    Quaternion operator*(const Quaternion& q) const;

    // Useful functions
    Quaternion conjugate() const;
    void normalize();
    double dot(const Quaternion& q) const;
};

Quaternion rotation_quaternion(double angle, const vec3& axis);
vec3 rotate(const Quaternion& q, const vec3& v, const Quaternion& q_inv);
Quaternion slerp(const Quaternion& q1, const Quaternion& q2, double t);

#endif
