#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"
#include <fstream>

// Colors
const color RED(1.0, 0.0, 0.0);
const color GREEN(0.0, 1.0, 0.0);
const color BLUE(0.0, 0.0, 1.0);
const color CYAN(0.0, 1.0, 1.0);
const color MAGENTA(1.0, 0.0, 1.0);
const color YELLOW(1.0, 1.0, 0.0);
const color WHITE(1.0, 1.0, 1.0);
const color BLACK(0.0, 0.0, 0.0);
const color BROWN(0.6, 0.3, 0.0);
const color PINK(1.0, 0.75, 0.8);
const color PURPLE(0.5, 0.0, 0.5);
const color GRAY(0.5, 0.5, 0.5);
const color ORANGE(1.0, 0.65, 0.0);
const color SKY_BLUE(0.53, 0.81, 0.98); // color(0.5, 0.7, 1.0)

inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return std::sqrt(linear_component);

    return 0;
}

void write_color(std::ofstream& file, const color& pixel_color)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Apply a linear to gamma transform for gamma 2
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Translate the [0,1] component values to the byte range [0,255].
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    // Write out the pixel color components.
    file << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}


#endif