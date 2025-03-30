#pragma once

// Headers
#include "core.hpp"

// Forward declarations
class Scene;
class Camera;
class ImageWriter;

struct LogWriter
{
    LogWriter();

    void write(const Scene& scene, const Camera& camera, const ImageWriter& image);
};

