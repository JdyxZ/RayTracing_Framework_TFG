#pragma once

// Headers
#include "hittable.hpp"
#include "core.hpp"
#include "vec.hpp"

// Forward declarations
class Ray;
class Texture;
class Interval;
class AABB;
class Material;

class constant_medium : public Hittable 
{
public:
    constant_medium(shared_ptr<Hittable> boundary, double density, shared_ptr<Texture> tex);
    constant_medium(shared_ptr<Hittable> boundary, double density, const color& albedo);

    bool hit(const shared_ptr<Ray>& r, Interval ray_t, shared_ptr<hit_record>& rec) const override;
    shared_ptr<AABB> bounding_box() const override;

private:
    shared_ptr<Hittable> boundary;
    double neg_inv_density;
    shared_ptr<Material> phase_function;
};


