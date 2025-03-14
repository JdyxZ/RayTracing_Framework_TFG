#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
public:
    virtual ~Texture() = default;

    virtual color value(pair<double, double> texture_coordinates, const point3& p) const = 0;
};

class solid_color : public Texture 
{
public:

    solid_color(const color& albedo) : albedo(albedo) {}
    solid_color(double red, double green, double blue) : solid_color(color(red, green, blue)) {}

    color value(pair<double,double> texture_coordinates, const point3& p) const override 
    {
        return albedo;
    }

private:
    color albedo;
};

class checker_texture : public Texture 
{
public:
    checker_texture(double scale, shared_ptr<Texture> even, shared_ptr<Texture> odd) 
        : inv_scale(1.0 / scale), even(even), odd(odd) {}

    checker_texture(double scale, const color& c1, const color& c2) 
        : checker_texture(scale, make_shared<solid_color>(c1), make_shared<solid_color>(c2)) {}

    color value(pair<double, double> texture_coordinates, const point3& p) const override
    {
        auto x = int(floor(inv_scale * p.x()));
        auto y = int(floor(inv_scale * p.y()));
        auto z = int(floor(inv_scale * p.z()));

        bool isEven = (x + y + z) % 2 == 0;

        return isEven ? even->value(texture_coordinates, p) : odd->value(texture_coordinates, p);
    }

private:
    double inv_scale;
    shared_ptr<Texture> even;
    shared_ptr<Texture> odd;
};

class image_texture : public Texture 
{
public:
    image_texture(const char* filename) : image(filename) {}

    color value(pair<double, double> texture_coordinates, const point3& p) const override
    {
        // Unwrap texture coordinates
        auto [u, v] = texture_coordinates;

        // If we have no texture data, then return solid cyan as a debugging aid.
        if (image.height() <= 0) return CYAN;

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = interval::unitary.clamp(u);
        v = 1.0 - interval::unitary.clamp(v);  // Flip V to image coordinates

        // Get pixel location
        auto i = int(u * image.width());
        auto j = int(v * image.height());

        // Read pixel data
        auto pixel = image.pixel_data(i, j);

        // Scale pixel color to 8 bit data
        auto color_scale = 1.0 / 255.0;
        return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

private:
    ImageReader image;
};

class noise_texture : public Texture 
{
public:
    noise_texture(double scale, int depth = 7) : scale(scale), depth(depth) {}

    color value(pair<double, double> texture_coordinates, const point3& p) const override
    {
        return color(.5, .5, .5) * (1 + std::sin(scale * p.z() + 10 * noise.turbulance(p, depth)));
    }

private:
    perlin noise;
    double scale = 1;
    int depth = 7;
};

#endif
