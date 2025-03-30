// Headers
#include "core.hpp"
#include "vec3.hpp"
#include "utilities.hpp"

vec3::vec3() : e{ 0,0,0 } {}

vec3::vec3(int i) : e{ (double)i, (double)i, (double)i } {}

vec3::vec3(double d) : e{ d, d, d } {}

vec3::vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

double vec3::x() const
{ 
    return e[0]; 
}

double vec3::y() const
{ 
    return e[1]; 
}

double vec3::z() const
{ 
    return e[2]; 
}

vec3 vec3::operator-() const
{ 
    return vec3(-e[0], -e[1], -e[2]); 
}

double vec3::operator[](int i) const
{ 
    return e[i]; 
}
double& vec3::operator[](int i)
{ 
    return e[i]; 
}

vec3& vec3::operator+=(double t)
{
    e[0] += t;
    e[1] += t;
    e[2] += t;
    return *this;
}

vec3& vec3::operator-=(double t)
{
    e[0] -= t;
    e[1] -= t;
    e[2] -= t;
    return *this;
}

vec3& vec3::operator*=(double t)
{
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

vec3& vec3::operator/=(double t)
{
    return *this *= 1 / t;
}

vec3& vec3::operator+=(const vec3& v)
{
    e[0] += v[0];
    e[1] += v[1];
    e[2] += v[2];
    return *this;
}

vec3& vec3::operator-=(const vec3& v)
{
    e[0] -= v[0];
    e[1] -= v[1];
    e[2] -= v[2];
    return *this;
}

vec3& vec3::operator*=(const vec3& v)
{
    e[0] *= v[0];
    e[1] *= v[1];
    e[2] *= v[2];
    return *this;
}

vec3& vec3::operator/=(const vec3& v)
{
    e[0] /= v[0];
    e[1] /= v[1];
    e[2] /= v[2];
    return *this;
}

std::ostream& operator<<(std::ostream& out, const vec3& v)
{
    return out << "[" << v.e[0] << ", " << v.e[1] << ", " << v.e[2] << "]";
}

double vec3::length() const
{
    return std::sqrt(length_squared());
}

double vec3::length_squared() const
{
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

vec3& vec3::normalize()
{
    double len = length();
    e[0] /= len;
    e[1] /= len;
    e[2] /= len;
    return *this;
}

bool vec3::near_zero() const
{
    return (std::fabs(e[0]) < kEpsilon) && (std::fabs(e[1]) < kEpsilon) && (std::fabs(e[2]) < kEpsilon);
}

vec3 vec3::random()
{
    return vec3(random_double(), random_double(), random_double());
}

vec3 vec3::random(double min, double max)
{
    return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

