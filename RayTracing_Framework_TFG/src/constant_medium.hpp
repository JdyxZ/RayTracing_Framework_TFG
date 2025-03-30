#pragma once

// Headers
#include "hittable.hpp"
#include "core.hpp"
#include "vec3.hpp"

// Forward declarations
class Ray;
class Texture;
class interval;
class aabb;
class Material;

class constant_medium : public Hittable 
{
public:
    constant_medium(shared_ptr<Hittable> boundary, double density, shared_ptr<Texture> tex);
    constant_medium(shared_ptr<Hittable> boundary, double density, const color& albedo);

    bool hit(const shared_ptr<Ray>& r, interval ray_t, shared_ptr<hit_record>& rec) const override;
    shared_ptr<aabb> bounding_box() const override;

private:
    shared_ptr<Hittable> boundary;
    double neg_inv_density;
    shared_ptr<Material> phase_function;
};


