// Headers
#include "core.hpp"
#include "vec.hpp"
#include "utilities.hpp"

// **************************** VEC 3 **************************** //

vec3::vec3() : x(0), y(0), z(0) {}

vec3::vec3(int i) : x(static_cast<double>(i)), y(static_cast<double>(i)), z(static_cast<double>(i)) {}

vec3::vec3(double d) : x(d), y(d), z(d) {}

vec3::vec3(double x, double y, double z) : x(x), y(y), z(z) {}

vec3::vec3(const vec4& v) : x(v.x), y(v.y), z(v.z) {}

vec3 vec3::operator-() const
{ 
    return vec3(-x, -y, -z); 
}

double vec3::operator[](int i) const
{ 
    switch (i)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        string error = Logger::error("vec4", "Invalid operator access value!!!");
        throw std::invalid_argument(error);
    }
}
double& vec3::operator[](int i)
{ 
    switch (i)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        string error = Logger::error("vec4", "Invalid operator access value!!!");
        throw std::invalid_argument(error);
    }
}

vec3& vec3::operator+=(double t)
{
    x += t;
    y += t;
    z += t;
    return *this;
}

vec3& vec3::operator-=(double t)
{
    x -= t;
    y -= t;
    z -= t;
    return *this;
}

vec3& vec3::operator*=(double t)
{
    x *= t;
    y *= t;
    z *= t;
    return *this;
}

vec3& vec3::operator/=(double t)
{
    return *this *= 1 / t;
}

vec3& vec3::operator+=(const vec3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

vec3& vec3::operator-=(const vec3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

vec3& vec3::operator*=(const vec3& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}

vec3& vec3::operator/=(const vec3& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const vec3& v)
{
    return out << "[" << v.x << ", " << v.y << ", " << v.z << "]";
}

double vec3::length() const
{
    return std::sqrt(length_squared());
}

double vec3::length_squared() const
{
    return x * x + y * y + z * z;
}

vec3& vec3::normalize()
{
    double len = length();
    x /= len;
    y /= len;
    z /= len;
    return *this;
}

bool vec3::near_zero() const
{
    return (std::fabs(x) < kEpsilon) && (std::fabs(y) < kEpsilon) && (std::fabs(z) < kEpsilon);
}

vec3 vec3::random()
{
    return vec3(random_double(), random_double(), random_double());
}

vec3 vec3::random(double min, double max)
{
    return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

// **************************** VEC 4 **************************** //

vec4::vec4() : x(0), y(0), z(0), w(0) {}

vec4::vec4(int i) : x(static_cast<double>(i)), y(static_cast<double>(i)), z(static_cast<double>(i)), w(static_cast<double>(i)) {}

vec4::vec4(double d) : x(d), y(d), z(d), w(d) {}

vec4::vec4(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

vec4::vec4(const vec3& v, double w) : x(v.x), y(v.y), z(v.z), w(w) {}

vec4 vec4::operator-() const
{
    return vec4(-x, -y, -z, -w);
}

double vec4::operator[](int i) const
{
    switch (i)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    default:
        string error = Logger::error("vec4", "Invalid operator access value!!!");
        throw std::invalid_argument(error);
    }
}

double& vec4::operator[](int i)
{
    switch (i)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    default:
        string error = Logger::error("vec4", "Invalid operator access value!!!");
        throw std::invalid_argument(error);
    }
}

vec4& vec4::operator+=(double t)
{
    x += t;
    y += t;
    z += t;
    w += t;
    return *this;
}

vec4& vec4::operator-=(double t)
{
    x -= t;
    y -= t;
    z -= t;
    w -= t;
    return *this;
}

vec4& vec4::operator*=(double t)
{
    x *= t;
    y *= t;
    z *= t;
    w *= t;
    return *this;
}

vec4& vec4::operator/=(double t)
{
    return *this *= 1 / t;
}

vec4& vec4::operator+=(const vec4& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}

vec4& vec4::operator-=(const vec4& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}

vec4& vec4::operator*=(const vec4& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return *this;
}

vec4& vec4::operator/=(const vec4& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const vec4& v)
{
    return out << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "]";
}

double vec4::length() const
{
    return sqrt(length_squared());
}

double vec4::length_squared() const
{
    return x * x + y * y + z * z + w * w;
}

vec4& vec4::normalize()
{
    double len = length();
    if (len > 0)
    {
        *this /= len;
    }
    return *this;
}

bool vec4::near_zero() const
{
    return (fabs(x) < kEpsilon) && (fabs(y) < kEpsilon) && (fabs(z) < kEpsilon) && (fabs(w) < kEpsilon);
}

vec4 vec4::random()
{
    return vec4(random_double(), random_double(), random_double(), random_double());
}

vec4 vec4::random(double min, double max)
{
    return vec4(random_double(min, max), random_double(min, max), random_double(min, max), random_double(min, max));
}

