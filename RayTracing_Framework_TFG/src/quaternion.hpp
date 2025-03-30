#pragma once

// Headers
#include "vec3.hpp"

struct Quaternion 
{
    double w, i, j, k;

    Quaternion(); // Default constructor: Identity quaternion
    Quaternion(double w, double i, double j, double k);
    Quaternion(const vec3& axis, const double angle); // Rotation quaternion

    static Quaternion identity();

    // Operators
    Quaternion operator*(const double scalar) const;
    Quaternion operator/(const double scalar) const;
	Quaternion operator+(const Quaternion& q) const;
	Quaternion operator-(const Quaternion& q) const;
    Quaternion operator*(const Quaternion& q) const;

    // Useful functions
    Quaternion conjugate() const;
    void normalize();
	vec3 rotate(const vec3& v) const;   
};

Quaternion normalize(Quaternion q);
double dot(const Quaternion& q1, const Quaternion& q2);
vec3 rotate3D(const Quaternion& q, const vec3& v, const Quaternion& q_inv);
vec3 rotate3D(const shared_ptr<Quaternion>& q, const vec3& v, const shared_ptr<Quaternion>& q_inv);
Quaternion slerp(const Quaternion& q1, const Quaternion& q2, double t);