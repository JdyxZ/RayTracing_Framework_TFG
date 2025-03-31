#pragma once

// Headers
#include "vec.hpp"
#include "core.hpp"

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
const color SKY_BLUE(135.0 / 255.0, 206.0 / 255.0, 235.0 / 255.0);

inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return std::sqrt(linear_component);

    return 0;
}

tuple<int, int, int> compute_color(const color& pixel_color);


