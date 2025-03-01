#ifndef HITTABLE_H
#define HITTABLE_H

// Forward declaration needed
class hittable;

enum PRIMITIVE
{
	SPHERE,
	TRIANGLE,
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
    shared_ptr<hittable> object;
    PRIMITIVE type = NOT_SPECIFIED;

    virtual ~hit_record() = default; 

    // Sets the hit record normal vector direction.
    void determine_normal_direction(const vec3& ray_direction, const vec3& outward_normal) 
    {
        // NOTE: The parameter `outward_normal` is assumed to have unit length.
        front_face = dot(ray_direction, outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class triangle_hit_record : public hit_record
{
public:
	std::optional<barycentric_coordinates> bc;

	triangle_hit_record() : bc(std::nullopt) 
    {
		type = TRIANGLE;
    }
};

class sphere_hit_record : public hit_record
{
public:
    sphere_hit_record()
    {
        type = SPHERE;
    }
};

class hittable {
public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval ray_t, shared_ptr<hit_record>& rec) const = 0;
    virtual const PRIMITIVE get_type() const = 0;
};

#endif
