#pragma once

// Headers
#include "core.hpp"

class vec3 
{
  public:
    double e[3];

    vec3();
    vec3(int i);
    vec3(double d);
    vec3(double e0, double e1, double e2);

    double x() const;
    double y() const;
    double z() const;

    vec3 operator-() const;
    double operator[](int i) const;
    double& operator[](int i);
    vec3& operator+=(double t);
    vec3& operator-=(double t);
    vec3& operator*=(double t);
    vec3& operator/=(double t);
    vec3& operator+=(const vec3& v);
    vec3& operator-=(const vec3& v);
    vec3& operator*=(const vec3& v);
    vec3& operator/=(const vec3& v);
    friend std::ostream& operator<<(std::ostream& out, const vec3& v);

    double length() const;
    double length_squared() const;
	vec3& normalize();
    bool near_zero() const; // Return true if the vector is close to zero in all dimensions.
    static vec3 random();
    static vec3 random(double min, double max);
    
};

// Operators overloads
inline vec3 operator+(const vec3& u, const vec3& v)
{
    return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v)
{
    return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v)
{
    return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline vec3 operator*(double t, const vec3& v)
{
    return vec3(t * v[0], t * v[1], t * v[2]);
}

inline vec3 operator*(const vec3& v, double t)
{
    return t * v;
}

inline vec3 operator/(const vec3& u, const vec3& v)
{
    return vec3(u[0] / v[0], u[1] / v[1], u[2] / v[2]);
}

inline vec3 operator/(const vec3& v, double t)
{
    return (1 / t) * v;
}

// Aliases
using point3 = vec3;
using color = vec3;
using normal = vec3;
using axis = vec3;


