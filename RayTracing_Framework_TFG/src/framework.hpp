#pragma once

// Headers
#include "vec.hpp"

// Axis definitions (right-handed coordinate system)
const axis x_axis = vec3(1, 0, 0);
const axis y_axis = vec3(0, 1, 0);
const axis z_axis = vec3(0, 0, -1);

const axis flipped_x_axis = vec3(-1, 0, 0);
const axis flipped_y_axis = vec3(0, -1, 0);
const axis flipped_z_axis = vec3(0, 0, 1);