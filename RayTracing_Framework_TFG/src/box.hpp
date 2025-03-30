#pragma once

// Headers
#include "vec3.hpp"
#include "hittable.hpp"

// Forward declaration
class Material;
class Ray;
class interval;
class bvh_node;
struct Chrono;

class Box : public Hittable
{
public:
	Box(const point3& p0, const point3& p1, shared_ptr<Material> material);

	bool hit(const shared_ptr<Ray>& r, interval ray_t, shared_ptr<hit_record>& rec) const override;
	shared_ptr<aabb> bounding_box() const override;
	const shared_ptr<Chrono> bvh_chrono() const;

private:
	vec3 p0, p1;
	shared_ptr<bvh_node> sides;
	shared_ptr<aabb> bbox;
	shared_ptr<Chrono> box_bvh_chrono;
};


