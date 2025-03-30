#pragma once

// Headers
#include "core.hpp"
#include "vec3.hpp"

// Forward declarations
class Ray;
class PDF;
class hit_record;
class Texture;

enum SCATTER_TYPE
{
    REFLECT,
    REFRACT
};

enum MATERIAL_TYPE
{
    LAMBERTIAN,
    METAL,
    DIELECTRIC,
    DIFFUSE_LIGHT,
    ISOTROPIC,
    NONE
};

struct scatter_record
{
public:
	bool is_specular;
	shared_ptr<Ray> specular_ray;
	shared_ptr<PDF> pdf;
	color attenuation;
    SCATTER_TYPE scatter_type;
};

class Material 
{
public:
    virtual ~Material() = default;

    virtual bool scatter(const shared_ptr<Ray>& incoming_ray, const shared_ptr<hit_record>& rec, scatter_record& srec) const;
    virtual color emitted(const shared_ptr<Ray>& incoming_ray, const shared_ptr<hit_record>& rec) const;
    virtual double scattering_pdf_value(const shared_ptr<Ray>& incoming_ray, const shared_ptr<hit_record>& rec, const shared_ptr<Ray>& scattered_ray) const;
    const MATERIAL_TYPE get_type() const;

protected:
    MATERIAL_TYPE type = NONE;
};

class lambertian : public Material 
{
public:
    lambertian(const color& albedo);
    lambertian(shared_ptr<Texture> texture);

    bool scatter(const shared_ptr<Ray>& incoming_ray, const shared_ptr<hit_record>& rec, scatter_record& srec) const override;
    double scattering_pdf_value(const shared_ptr<Ray>& incoming_ray, const shared_ptr<hit_record>& rec, const shared_ptr<Ray>& scattered_ray) const override;

private:
    shared_ptr<Texture> texture;
};

class metal : public Material 
{
public:
    metal(const color& albedo, double fuzz);

    bool scatter(const shared_ptr<Ray>& incoming_ray, const shared_ptr<hit_record>& rec, scatter_record& srec) const override;

private:
    color albedo;
    double fuzz;
};

class dielectric : public Material 
{
public:
    dielectric(double refraction_index);

    bool scatter(const shared_ptr<Ray>& incoming_ray, const shared_ptr<hit_record>& rec, scatter_record& srec) const override;

private:
    double refraction_index; // Refractive index in vacuum or air, or the ratio of the material's refractive index over the refractive index of the enclosing media

    static double reflectance(double cosine, double refraction_index);
};

class diffuse_light : public Material 
{
public:
    diffuse_light(shared_ptr<Texture> texture);
    diffuse_light(const color& emit);

    color emitted(const shared_ptr<Ray>& incoming_ray, const shared_ptr<hit_record>& rec) const override;

private:
    shared_ptr<Texture> texture;
};

class isotropic : public Material 
{
public:
    isotropic(const color& albedo);
    isotropic(shared_ptr<Texture> texture);

    bool scatter(const shared_ptr<Ray>& incoming_ray, const shared_ptr<hit_record>& rec, scatter_record& srec) const override;
    double scattering_pdf_value(const shared_ptr<Ray>& incoming_ray, const shared_ptr<hit_record>& rec, const shared_ptr<Ray>& scattered_ray) const override;

private:
    shared_ptr<Texture> texture;
};


