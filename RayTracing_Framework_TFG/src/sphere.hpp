#pragma once

// Headers
#include "hittable.hpp"
#include "ray.hpp"

class Sphere : public Hittable
{
public:
    Sphere(const point3& static_center, const double radius, shared_ptr<Material> material, bool pdf = false); // Stationary sphere
    Sphere(const point3& start_center, const point3& end_center, const double radius, shared_ptr<Material> material); // Moving sphere

    bool hit(const shared_ptr<Ray>& r, interval ray_t, shared_ptr<hit_record>& rec) const override;
    shared_ptr<aabb> bounding_box() const override;
    double pdf_value(const point3& origin, const vec3& direction) const override;
    vec3 random_scattering_ray(const point3& origin) const override;

private:
    motion_vector center;
    double radius;
    shared_ptr<Material> material;
    shared_ptr<aabb> bbox;

    static pair<double, double> get_sphere_uv(const point3& p);
    static vec3 sphere_front_face_random(double radius, double distance_squared);
};


