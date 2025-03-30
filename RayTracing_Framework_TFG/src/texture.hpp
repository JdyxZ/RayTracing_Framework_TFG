#pragma once

// Headers
#include "vec3.hpp"
#include "core.hpp"

// Forward declarations
class ImageReader;
class Perlin;

class Texture 
{
public:
    virtual ~Texture() = default;
    virtual color value(pair<double, double> texture_coordinates, const point3& p) const = 0;
};

class solid_color : public Texture 
{
public:
    solid_color(const color& albedo);
    solid_color(double red, double green, double blue);

    color value(pair<double,double> texture_coordinates, const point3& p) const override;

private:
    color albedo;
};

class checker_texture : public Texture 
{
public:
    checker_texture(double scale, shared_ptr<Texture> even, shared_ptr<Texture> odd);
    checker_texture(double scale, const color& c1, const color& c2);

    color value(pair<double, double> texture_coordinates, const point3& p) const override;

private:
    double inv_scale;
    shared_ptr<Texture> even;
    shared_ptr<Texture> odd;
};

class image_texture : public Texture 
{
public:
    image_texture(const char* filename);
    image_texture(string filename);

    color value(pair<double, double> texture_coordinates, const point3& p) const override;

private:
    shared_ptr<ImageReader> image;
};

class noise_texture : public Texture 
{
public:
    noise_texture(double scale, int depth = 7);

    color value(pair<double, double> texture_coordinates, const point3& p) const override;

private:
    shared_ptr<Perlin> noise;
    double scale = 1;
    int depth = 7;
};


