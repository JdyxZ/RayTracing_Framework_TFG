﻿#pragma once

// Headers
#include "vec.hpp"
#include "core.hpp"

// Forward declarations
struct Chrono;
class Scene;
class ImageWriter;
class Ray;
class Triangle;
class triangle_hit_record;

class Camera 
{
public:
    // Camera settings
    double vertical_fov = 90;           // Vertical view angle (field of view)
    double defocus_angle = 0;           // Angle of the cone with apex at viewport center and base at camera center (defocus disk)
    double focus_distance = 10;         // Distance from camera lookfrom point to plane of perfect focus

	// Viewport settings
    double viewport_height;	            // Height of viewport
	double viewport_width;              // Width of viewport

	// Camera position and orientation
    point3 lookfrom = point3(0, 0, 0);  // Point camera is looking from
    point3 lookat = point3(0, 0, -1);   // Point camera is looking at
    vec3   world_up = vec3(0, 1, 0);    // Camera-relative "up" direction

    // Benchmark
    int primary_rays = 0;
    int reflected_rays = 0;
    int refracted_rays = 0;
    int rays_casted = 0;
    int average_rays_per_second = 0;
    shared_ptr<Chrono> render_chrono;

    Camera();

    void initialize(const Scene& scene, const ImageWriter& image);
    void render(Scene& scene, ImageWriter& image);

private:

    // Internal variables
    point3 pixel00_loc;             // Location of pixel 0, 0
    vec3   pixel_delta_u;           // Offset to pixel to the right
    vec3   pixel_delta_v;           // Offset to pixel below
    vec3   side, up, view;          // Camera frame basis vectors
    vec3   defocus_disk_u;          // Defocus disk horizontal radius
    vec3   defocus_disk_v;          // Defocus disk vertical radius    
	int    pixel_sample_sqrt;	    // Square root of samples per pixel
	double pixel_sample_sqrt_inv;   // Inverse of square root of samples per pixel

    const shared_ptr<Ray> get_ray_sample(int pixel_row, int pixel_column, int sample_row, int sample_column) const; // Construct a camera ray originating from the defocus disk and directed at randomly sampled point around the pixel location pixel_row, pixel_column for stratified sample square sample_row, sample_column.
    color ray_color(const shared_ptr<Ray>& sample_ray, int depth, const Scene& scene);
    color sky_blend(const shared_ptr<Ray>& r) const;
    optional<color> barycentric_color_interpolation(const shared_ptr<triangle_hit_record>& rec, Triangle* t) const;

};


