#ifndef VEC3_H
#define VEC3_H

class vec3 {
  public:
    double e[3];

    vec3() : e{0,0,0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3& v) 
    {
        e[0] += v[0];
        e[1] += v[1];
        e[2] += v[2];
        return *this;
    }

    vec3& operator-=(const vec3& v) 
    {
        e[0] -= v[0];
        e[1] -= v[1];
        e[2] -= v[2];
        return *this;
    }

    vec3& operator*=(double t) 
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(const vec3& v) 
    {
        e[0] /= v[0];
        e[1] /= v[1];
        e[2] /= v[2];
        return *this;
    }

    vec3& operator/=(double t) 
    {
        return *this *= 1/t;
    }

    double length() const 
    {
        return std::sqrt(length_squared());
    }

    double length_squared() const 
    {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

	vec3& normalize()
	{
		double len = length();
		e[0] /= len;
		e[1] /= len;
		e[2] /= len;
		return *this;
	}

    // Return true if the vector is close to zero in all dimensions.
    bool near_zero() const 
    {
        return (std::fabs(e[0]) < kEpsilon) && (std::fabs(e[1]) < kEpsilon) && (std::fabs(e[2]) < kEpsilon);
    }

    static vec3 random() 
    {
        return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random(double min, double max) 
    {
        return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
    }
};

// Vector utility functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) 
{
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

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

inline vec3 operator/=(const vec3& u, const vec3& v)
{
    return vec3(u[0] / v[0], u[1] / v[1], u[2] / v[2]);
}

inline vec3 operator/(const vec3& v, double t) 
{
    return (1/t) * v;
}

// Aliases
using point3 = vec3;
using color = vec3;
using normal = vec3;


#endif
