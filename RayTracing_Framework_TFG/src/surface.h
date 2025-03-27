#ifndef SURFACE_H
#define SURFACE_H

class Surface : public Hittable
{
public:
	Surface() {}

	Surface(const shared_ptr<hittable_list>& triangles, const shared_ptr<Material>& material) : material(material)
	{
		_num_triangles = int(triangles->size());
		this->triangles = make_shared<bvh_node>(*triangles);
		bbox = this->triangles->bounding_box();
		surface_bvh_chrono = this->triangles->bvh_chrono();
	}

	bool hit(const Ray& r, interval ray_t, shared_ptr<hit_record>& rec) const override
	{
		return triangles->hit(r, ray_t, rec);
	}

	aabb bounding_box() const override
	{
		return bbox;
	}

	const Chrono& bvh_chrono() const
	{
		return surface_bvh_chrono;
	}

	const int& num_triangles() const
	{
		return _num_triangles;
	}

private:
	shared_ptr<bvh_node> triangles;
	shared_ptr<Material> material;
	aabb bbox;
	Chrono surface_bvh_chrono;
	int _num_triangles = 0;
};

#endif

