#ifndef CORE_H
#define CORE_H

// Directory names
const string textures_directory = "images";
const string models_directory = "models";
const string output_directory = "render";
const string logs_directory = "logs";

// Paths
const string cwd = fs::current_path().string();
const string textures_path = cwd + "\\" + textures_directory + "\\";
const string models_path = cwd + "\\" + models_directory + "\\";
const string output_path = cwd + "\\" + output_directory + "\\";
const string logs_path = cwd + "\\" + logs_directory + "\\";

// Constants
constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;
constexpr double kEpsilon = 1e-8;
constexpr double practically_zero = 1e-160;

// Utility strucs
struct file_size
{
    double amount;
    string unit;
};

// Utility methods
file_size get_file_size(const string& file_path)
{
    if (!fs::exists(file_path)) 
    {
        Logger::error("CORE", "File does not exist:" + file_path);
        return { 0.0, "B" };  
    }

    const uintmax_t bytes = fs::file_size(file_path);

    constexpr double kb_factor = 1024.0;
    constexpr double mb_factor = kb_factor * 1024.0;
    constexpr double gb_factor = mb_factor * 1024.0;

    // Check GB first
    if (double gb_size = static_cast<double>(bytes) / gb_factor; gb_size >= 1.0)
        return { std::floor(gb_size * 100) / 100, "GB" };
    // Then MB
    if (double mb_size = static_cast<double>(bytes) / mb_factor; mb_size >= 1.0)
        return { std::floor(mb_size * 100) / 100, "MB" };
    // Then KB
    if (double kb_size = static_cast<double>(bytes) / kb_factor; kb_size >= 1.0) 
        return { std::floor(kb_size * 100) / 100, "KB" };

    // Default to bytes
    return { static_cast<double>(bytes), "B" };
}

inline string to_list(const vector<string>& vec) 
{
    std::ostringstream oss;

    for (size_t i = 0; i < vec.size(); ++i) 
    {
        oss << vec[i];
        if (i != vec.size() - 1) 
            oss << ", "; 
    }

    return oss.str();
}

inline string trim(const string& str) 
{
    auto start = std::find_if_not(str.begin(), str.end(), ::isspace);
    auto end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();
    return (start < end) ? string(start, end) : string();
}

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

inline constexpr bool is_valid_timestamp_format(const char* fmt)
{
    // Nullptr check
    if (!fmt) return false;

    // Valid chars inside the format string
    constexpr const char* valid_specifiers = "aAbBcCdDeFgGhHIjmMnOpPrRsSuUVwWxXyYzZ";
    constexpr const char* valid_separators = "-_:/.· ";

    while (*fmt)
    {
        if (*fmt == '%')
        {
            fmt++; // Move to the next character after '%'

            if (!*fmt || !strchr("aAbBcCdDeFgGhHIjmMnOpPrRsSuUVwWxXyYzZ", *fmt))
                return false;
        }
        else if (!strchr(valid_separators, *fmt))
        {
            return false;
        }

        fmt++;
    }
    return true;
}

inline string get_current_timestamp(const char* fmt = "%a %b %d %H_%M_%S %Y", bool miliseconds = false)
{
    // Validate timestamp format
    if (!is_valid_timestamp_format(fmt))
    {
        Logger::error("CORE", "Invalid format time!");
        return string("ERROR get_current_timestamp");
    }

    // Get the current local date and time
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    // Use localtime_s for safer local time conversion
    std::tm localTime;
    if (localtime_s(&localTime, &now_time_t) != 0)
    {
        Logger::error("CORE", "Could not retrieve local time!");
        return string("ERROR get_current_timestamp");
    }

    // Format the time using the provided format
    std::ostringstream oss;
    oss << std::put_time(&localTime, fmt);

    // Format miliseconds if requested
    if (miliseconds)
        oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    return oss.str();
}


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

