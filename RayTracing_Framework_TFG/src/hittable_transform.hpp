#pragma once

// Headers
#include "hittable.hpp"

// Forward declarators
struct Quaternion;

class translate : public Hittable 
{
public:
    translate(shared_ptr<Hittable> object, const vec3& offset);
    bool hit(const shared_ptr<Ray>& r, Interval ray_t, shared_ptr<hit_record>& rec) const override;
    shared_ptr<AABB> bounding_box() const override;

private:
    shared_ptr<Hittable> object;
    vec3 offset;
    shared_ptr<AABB> bbox;

    shared_ptr<AABB> compute_translated_bbox();
};

class rotate : public Hittable
{
public:
    rotate(shared_ptr<Hittable> object, vec3 axis, double angle);
    bool hit(const shared_ptr<Ray>& r, Interval ray_t, shared_ptr<hit_record>& rec) const override;
    shared_ptr<AABB> bounding_box() const override;

private:
    shared_ptr<Hittable> object;
    shared_ptr<Quaternion> rotation_quat;
    shared_ptr<Quaternion> inverse_rotation_quat;
    shared_ptr<AABB> bbox;

    shared_ptr<AABB> compute_rotated_bbox() const;
};

class scale : public Hittable
{
public:
    scale(shared_ptr<Hittable> object, const vec3& scale_factor);
    bool hit(const shared_ptr<Ray>& r, Interval ray_t, shared_ptr<hit_record>& rec) const override;
    shared_ptr<AABB> bounding_box() const override;

private:
    shared_ptr<Hittable> object;
    vec3 scale_factor;
    vec3 inverse_scale;
    shared_ptr<AABB> bbox;

    shared_ptr<AABB> compute_scaled_bbox() const;
};


