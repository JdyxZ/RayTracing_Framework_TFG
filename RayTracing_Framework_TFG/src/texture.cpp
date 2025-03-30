// Headers
#include "core.hpp"
#include "texture.hpp"
#include "perlin.hpp"
#include "image_reader.hpp"
#include "color.hpp"
#include "interval.hpp"

solid_color::solid_color(const color& albedo) : albedo(albedo) {}

solid_color::solid_color(double red, double green, double blue) : solid_color(color(red, green, blue)) {}

color solid_color::value(pair<double, double> texture_coordinates, const point3& p) const
{
    return albedo;
}

checker_texture::checker_texture(double scale, shared_ptr<Texture> even, shared_ptr<Texture> odd) 
    : inv_scale(1.0 / scale), even(even), odd(odd) {}

checker_texture::checker_texture(double scale, const color& c1, const color& c2)
    : checker_texture(scale, make_shared<solid_color>(c1), make_shared<solid_color>(c2)) {}

color checker_texture::value(pair<double, double> texture_coordinates, const point3& p) const
{
    auto x = int(floor(inv_scale * p.x()));
    auto y = int(floor(inv_scale * p.y()));
    auto z = int(floor(inv_scale * p.z()));

    bool isEven = (x + y + z) % 2 == 0;

    return isEven ? even->value(texture_coordinates, p) : odd->value(texture_coordinates, p);
}

image_texture::image_texture(const char* filename) 
{
    image = make_shared<ImageReader>(filename);
}

image_texture::image_texture(string filename)
{
    image = make_shared<ImageReader>(filename.c_str());
}

color image_texture::value(pair<double, double> texture_coordinates, const point3& p) const
{
    // Unwrap texture coordinates
    auto [u, v] = texture_coordinates;

    // If we have no texture data, then return solid cyan as a debugging aid.
    if (image->height() <= 0) return CYAN;

    // Clamp input texture coordinates to [0,1] x [1,0]
    u = interval::unitary.clamp(u);
    v = 1.0 - interval::unitary.clamp(v);  // Flip V to image coordinates

    // Get pixel location
    auto i = int(u * image->width());
    auto j = int(v * image->height());

    // Read pixel data
    auto pixel = image->pixel_data(i, j);

    // Scale pixel color to 8 bit data
    auto color_scale = 1.0 / 255.0;
    return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
}

noise_texture::noise_texture(double scale, int depth) : scale(scale), depth(depth) 
{
    noise = make_shared<Perlin>();
}

color noise_texture::value(pair<double, double> texture_coordinates, const point3& p) const
{
    return color(.5, .5, .5) * (1 + std::sin(scale * p.z() + 10 * noise->turbulance(p, depth)));
}