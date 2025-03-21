#ifndef CORE_H
#define CORE_H

// External Headers
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <string>
#include <optional>
#include <tuple>
#include <algorithm>
#include <vector>

// Constants
constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;
constexpr double kEpsilon = 1e-8;
constexpr double practically_zero = 1e-160;

// C++ Std Usings
using std::make_shared;
using std::make_pair;
using std::shared_ptr;
using std::nullopt;
using std::optional;
using std::string;
using std::vector;
using std::tuple;
using std::pair;
using std::floor;

// Utility methods
inline double clamp(double value, double min, double max) 
{
    if (min > max) std::swap(min, max);
    return std::max(min, std::min(value, max));
}

inline double degrees_to_radians(double degrees) 
{
    return degrees * pi / 180.0;
}

// Returns a random real in [0,1).
inline double random_double() 
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

// Returns a random real in [min,max).
inline double random_double(double min, double max) 
{
    return min + (max - min) * random_double();
}

// Returns a random integer in [min,max].
inline int random_int(int min, int max) 
{
    return int(random_double(min, max + 1));
}

// Common Headers
#include "interval.h"
#include "ray.h"
#include "vec3.h"
#include "color.h"
#include "quaternion.h"
#include "matrix.h"

// Axis definitions (right-handed coordinate system)
const axis x_axis = vec3(1, 0, 0);
const axis y_axis = vec3(0, 1, 0);
const axis z_axis = vec3(0, 0, -1);

// vec3 utility methods
inline double dot(const vec3& u, const vec3& v)
{
    return u[0] * v[0]
        + u[1] * v[1]
        + u[2] * v[2];
}

inline vec3 cross(const vec3& u, const vec3& v)
{
    return vec3(u[1] * v[2] - u[2] * v[1],
        u[2] * v[0] - u[0] * v[2],
        u[0] * v[1] - u[1] * v[0]);
}


inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

// Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
vec3 sample_square()
{
    return vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

// Returns the vector to a random point in the square sub-pixel specified by grid
// indices sample_row and sample_column, for an idealized unit square pixel [-.5,-.5] to [+.5,+.5].
vec3 sample_square_stratified(int sample_row, int sample_column, double pixel_sample_sqrt_inv)
{
    auto px = ((sample_row + random_double()) * pixel_sample_sqrt_inv) - 0.5;
    auto py = ((sample_column + random_double()) * pixel_sample_sqrt_inv) - 0.5;

    return vec3(px, py, 0);
}

// Returns a random point in the unit disk.
inline vec3 random_in_unit_disk()
{
    while (true)
    {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

// Returns a random point in the disk centered at origin.
vec3 sample_disk(double radius)
{
    return radius * random_in_unit_disk();
}

// Returns a random point in the defocus disk given.
point3 defocus_disk_sample(vec3 center, vec3 defocus_disk_u, vec3 defocus_disk_v)  
{
    auto p = random_in_unit_disk();
    return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
}

inline vec3 random_unit_vector()
{
    while (true)
    {
        auto p = vec3::random(-1, 1);
        auto lensq = p.length_squared();
        if (practically_zero < lensq && lensq <= 1.0)
            return p / sqrt(lensq);
    }
}

inline vec3 random_on_hemisphere(const vec3& normal)
{
    vec3 unit_vector_on_sphere = random_unit_vector();

    // If the vector is in the same hemisphere than the normal return it, otherwise return its opposite.
    return (dot(unit_vector_on_sphere, normal) > 0.0) ? unit_vector_on_sphere : -unit_vector_on_sphere;
}

inline vec3 random_cosine_hemisphere_direction() 
{
    auto r1 = random_double();
    auto r2 = random_double();

    auto phi = 2 * pi * r1;
    auto x = std::cos(phi) * std::sqrt(r2);
    auto y = std::sin(phi) * std::sqrt(r2);
    auto z = std::sqrt(1 - r2);

    return vec3(x, y, z);
}

inline vec3 reflect(const vec3& v, const vec3& n)
{
    return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3& incoming_ray, const vec3& n, double cos_theta, double refraction_index)
{
    vec3 refracted_ray_perpendicular = refraction_index * (incoming_ray + cos_theta * n);
    vec3 refracted_ray_parellel = -std::sqrt(std::fabs(1.0 - refracted_ray_perpendicular.length_squared())) * n;
    return refracted_ray_perpendicular + refracted_ray_parellel;
}

inline vec3 lerp(double a, vec3 start, vec3 end)
{
    return (1.0 - a) * start + a * end;
}

#endif

