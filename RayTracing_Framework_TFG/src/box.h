#ifndef BOX_H
#define BOX_H

class Box : public Hittable
{
public:

	Box(const point3& p0, const point3& p1, shared_ptr<Material> material) 
	{
		// Validate that p0 and p1 are not aligned in any coordinate (this would define a line or a point instead of a box)
		if (p0.x() == p1.x() || p0.y() == p1.y() || p0.z() == p1.z())
		{
			string error = Logger::error("BOX", "Points p0 and p1 are aligned in at least one coordinate. A box cannot be formed.");
			throw std::invalid_argument(error);
		}

		// Define box
		type = BOX;
		this->p0 = p0;
		this->p1 = p1;

		// Construct the two opposite vertices with the minimum and maximum coordinates.
		auto min = point3(std::fmin(p0.x(), p1.x()), std::fmin(p0.y(), p1.y()), std::fmin(p0.z(), p1.z()));
		auto max = point3(std::fmax(p0.x(), p1.x()), std::fmax(p0.y(), p1.y()), std::fmax(p0.z(), p1.z()));
		
		// Construct the coordinate system of the box.
		auto dx = vec3(max.x() - min.x(), 0, 0);
		auto dy = vec3(0, max.y() - min.y(), 0);
		auto dz = vec3(0, 0, max.z() - min.z());

		// Construct the box from the six quads that make up its sides.
		auto quad1 = make_shared<Quad>(point3(min.x(), min.y(), max.z()), dx, dy, material); // front
		auto quad2 = make_shared<Quad>(point3(max.x(), min.y(), max.z()), -dz, dy, material); // right
		auto quad3 = make_shared<Quad>(point3(max.x(), min.y(), min.z()), -dx, dy, material); // back
		auto quad4 = make_shared<Quad>(point3(min.x(), min.y(), min.z()), dz, dy, material); // left
		auto quad5 = make_shared<Quad>(point3(min.x(), max.y(), max.z()), dx, -dz, material); // top
		auto quad6 = make_shared<Quad>(point3(min.x(), min.y(), min.z()), dx, dz, material); // bottom
		
		// BVH
		auto sides_list = hittable_list();
		sides_list.add(quad1);
		sides_list.add(quad2);
		sides_list.add(quad3);
		sides_list.add(quad4);
		sides_list.add(quad5);
		sides_list.add(quad6);
		sides = make_shared<bvh_node>(sides_list);
		box_bvh_chrono = sides->bvh_chrono();

		// Construct the bounding box of the box
		interval x = interval(min.x(), max.x());
		interval y = interval(min.y(), max.y());
		interval z = interval(min.z(), max.z());
		bbox = aabb(x, y, z);
	}

	bool hit(const Ray& r, interval ray_t, shared_ptr<hit_record>& rec) const override
	{
		return sides->hit(r, ray_t, rec);
	}

	aabb bounding_box() const override
	{
		return bbox;
	}

	const Chrono& bvh_chrono() const
	{
		return box_bvh_chrono;
	}

private:
	vec3 p0, p1;
	shared_ptr<bvh_node> sides;
	aabb bbox;
	Chrono box_bvh_chrono;
};

#endif
