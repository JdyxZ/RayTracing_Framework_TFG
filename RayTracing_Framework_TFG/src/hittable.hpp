﻿#pragma once

// Headers
#include "core.hpp"
#include "vec3.hpp"
#include "matrix.hpp"

// Forward declarations
class Material;
class Ray;
class Interval;
class AABB;

enum PRIMITIVE
{
	SPHERE,
	TRIANGLE,
    QUAD,
    BOX,
    MESH,
    BVH_NODE,
	NOT_SPECIFIED
};

struct barycentric_coordinates
{
	double u, v, w;
};

class hit_record 
{
public:
    point3 p;
    vec3 normal;
    double t;
    bool front_face;
    shared_ptr<Material> material;
    pair<double, double> texture_coordinates;
    PRIMITIVE type = NOT_SPECIFIED;

    virtual ~hit_record() = default; 

    void determine_normal_direction(const vec3& ray_direction, const vec3& outward_normal);     // Sets the hit record normal vector direction.
};

class sphere_hit_record : public hit_record
{
public:
    sphere_hit_record();
};

class triangle_hit_record : public hit_record
{
public:
	optional<barycentric_coordinates> bc;

    triangle_hit_record();
};

class quad_hit_record : public hit_record
{
public:
    quad_hit_record();
};

class Hittable
{
public:
    Hittable();
    virtual ~Hittable() = default;

    virtual bool hit(const shared_ptr<Ray>& r, Interval ray_t, shared_ptr<hit_record>& rec) const = 0;
    virtual shared_ptr<AABB> bounding_box() const = 0;
    virtual double pdf_value(const point3& hit_point, const vec3& scattering_direction) const;
    virtual vec3 random_scattering_ray(const point3& hit_point) const;
    const PRIMITIVE get_type() const;
    const bool has_pdf() const;

protected:
    PRIMITIVE type = NOT_SPECIFIED;
    shared_ptr<Matrix44> model = make_shared<Matrix44>(Matrix::identity(4));
    bool pdf = false;
};


