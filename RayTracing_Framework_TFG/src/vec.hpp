#pragma once

// Headers
#include "core.hpp"

class vec {
public:
    virtual ~vec() = default; 

    virtual double length() const = 0;
    virtual double length_squared() const = 0;
    virtual vec& normalize() = 0;
    virtual bool near_zero() const = 0;
};

class vec4;

class vec3 : public vec 
{
  public:
    double x, y, z;

    // Constructors
    vec3();
    vec3(int i);
    vec3(double d);
    vec3(double x, double y, double z);
    vec3(const vec4& v);

    // Operator overloads
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

    // Length-related functions
    double length() const override;
    double length_squared() const override;
	vec3& normalize() override;
    bool near_zero() const override; // Return true if the vector is close to zero in all dimensions.

    // Static random vector generation
    static vec3 random();
    static vec3 random(double min, double max);
    
};

class vec4 : public vec
{
public:
    double x, y, z, w;

    // Constructors
    vec4();
    vec4(int i);
    vec4(double d);
    vec4(double x, double y, double z, double w);
    vec4(const vec3& v, double w);

    // Operator overloads
    vec4 operator-() const;
    double operator[](int i) const;
    double& operator[](int i);
    vec4& operator+=(double t);
    vec4& operator-=(double t);
    vec4& operator*=(double t);
    vec4& operator/=(double t);
    vec4& operator+=(const vec4& v);
    vec4& operator-=(const vec4& v);
    vec4& operator*=(const vec4& v);
    vec4& operator/=(const vec4& v);
    friend std::ostream& operator<<(std::ostream& out, const vec4& v);

    // Length-related functions
    double length() const override;
    double length_squared() const override;
    vec4& normalize() override;
    bool near_zero() const override; // Return true if the vector is close to zero in all dimensions.

    // Static random vector generation
    static vec4 random();
    static vec4 random(double min, double max);
};

// vec3 operator overloads
inline vec3 operator+(const vec3& u, const vec3& v)
{
    return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline vec3 operator-(const vec3& u, const vec3& v)
{
    return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline vec3 operator*(const vec3& u, const vec3& v)
{
    return vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline vec3 operator*(double t, const vec3& v)
{
    return vec3(t * v.x, t * v.y, t * v.z);
}

inline vec3 operator*(const vec3& v, double t)
{
    return t * v;
}

inline vec3 operator/(const vec3& u, const vec3& v)
{
    return vec3(u.x / v.x, u.y / v.y, u.z / v.z);
}

inline vec3 operator/(const vec3& v, double t)
{
    return (1 / t) * v;
}

// vec4 operators overloads
inline vec4 operator+(const vec4& u, const vec4& v)
{
    return vec4(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w);
}

inline vec4 operator-(const vec4& u, const vec4& v)
{
    return vec4(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w);
}

inline vec4 operator*(const vec4& u, const vec4& v)
{
    return vec4(u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w);
}

inline vec4 operator*(double t, const vec4& v)
{
    return vec4(t * v.x, t * v.y, t * v.z, t * v.w);
}

inline vec4 operator*(const vec4& v, double t)
{
    return t * v;
}

inline vec4 operator/(const vec4& u, const vec4& v)
{
    return vec4(u.x / v.x, u.y / v.y, u.z / v.z, u.w / v.w);
}

inline vec4 operator/(const vec4& v, double t)
{
    return (1 / t) * v;
}

// Aliases
using point3 = vec3;
using color = vec3;
using normal = vec3;
using axis = vec3;
using point4 = vec4;

