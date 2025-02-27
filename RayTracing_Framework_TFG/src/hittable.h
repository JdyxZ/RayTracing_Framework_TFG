#ifndef HITTABLE_H
#define HITTABLE_H

#define CULLING false;

#include <optional>

enum PRIMITIVE
{
	SPHERE,
	TRIANGLE
};

struct barycentric_coordinates
{
	double u, v, w;
};

class hit_record {
public:
    point3 p;
    vec3 normal;
    double t;
    bool front_face;
    std::optional<barycentric_coordinates> bc;

    // Sets the hit record normal vector direction.
    void determine_normal_direction(const vec3& ray_direction, const vec3& outward_normal) 
    {
        // NOTE: The parameter `outward_normal` is assumed to have unit length.
        front_face = dot(ray_direction, outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
    virtual const PRIMITIVE get_type() const = 0;
};

#endif
