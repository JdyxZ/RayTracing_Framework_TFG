#pragma once

// Headers
#include "hittable.hpp"
#include "vec.hpp"

// Forward declaration
class Material;
class Ray;
class Interval;
class bvh_node;
struct Chrono;
class Matrix44;

class Box : public Hittable
{
public:
	Box(point3 p0, point3 p1, const shared_ptr<Material>& material, const shared_ptr<Matrix44>& model = nullptr);

	bool hit(const shared_ptr<Ray>& r, Interval ray_t, shared_ptr<hit_record>& rec) const override;
	shared_ptr<AABB> bounding_box() const override;
	const shared_ptr<Chrono> bvh_chrono() const;

private:
	vec3 p0, p1;
	shared_ptr<Material> material;
	shared_ptr<bvh_node> sides;
	shared_ptr<AABB> bbox;
	shared_ptr<Chrono> box_bvh_chrono;
};


