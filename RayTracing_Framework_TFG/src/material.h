#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material 
{
public:
    virtual ~material() = default;

    virtual bool scatter(const ray& incoming_ray, const shared_ptr<hit_record>& rec, color& attenuation, ray& outgoing_ray) const 
    {
        return false;
    }
};

class lambertian : public material 
{
public:
    lambertian(const color& albedo) : albedo(albedo) {}

    bool scatter(const ray& incoming_ray, const shared_ptr<hit_record>& rec, color& attenuation, ray& scattered_ray) const override
    {
		// Generate random scatter direction
        auto scatter_direction = rec->normal + random_unit_vector();

        // Intercept degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = rec->normal;

		// Create scattered ray
        scattered_ray = ray(rec->p, scatter_direction, incoming_ray.time());
        attenuation = albedo;

        return true;
    }

private:
    color albedo;
};

class metal : public material 
{
public:
    metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& incoming_ray, const shared_ptr<hit_record>& rec, color& attenuation, ray& reflected_ray) const override
    {
		// Reflect the incoming ray
        vec3 reflected = reflect(incoming_ray.direction(), rec->normal);
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());

		// Create reflected ray
        reflected_ray = ray(rec->p, reflected, incoming_ray.time());
        attenuation = albedo;

		// Absorb the ray if it's reflected into the surface
        return dot(reflected_ray.direction(), rec->normal) > 0;
    }

private:
    color albedo;
    double fuzz;
};

class dielectric : public material {
public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const ray& incoming_ray, const shared_ptr<hit_record>& rec, color& attenuation, ray& scattered_ray) const override
    {
        // Attenuation is always 1 (the glass surface absorbs nothing)
        attenuation = color(1.0, 1.0, 1.0);

        // Check refractive index order
        double ri = rec->front_face ? (1.0 / refraction_index) : refraction_index;

		// Calculate cosinus and sinus of theta (angle between the ray and the normal)
        vec3 unit_direction = unit_vector(incoming_ray.direction());
        double cos_theta = std::fmin(dot(-unit_direction, rec->normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

		// Check if there is total reflection (the ray cannot refract)
        bool cannot_refract = ri * sin_theta > 1.0;

		// Check reflectance probability
		double reflect_prob = reflectance(cos_theta, ri);

		// Check if the ray should reflect or refract
		vec3 scattering_direction = cannot_refract || reflect_prob > random_double() ? reflect(unit_direction, rec->normal) : refract(unit_direction, rec->normal, cos_theta, ri);

		// Create scattered ray
        scattered_ray = ray(rec->p, scattering_direction, incoming_ray.time());
        return true;
    }

private:
    double refraction_index; // Refractive index in vacuum or air, or the ratio of the material's refractive index over the refractive index of the enclosing media

    static double reflectance(double cosine, double refraction_index) 
    {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

#endif
