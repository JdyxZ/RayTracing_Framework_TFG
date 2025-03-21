#ifndef SURFACE_H
#define SURFACE_H

class Surface : public Hittable
{
public:
	Surface() {}

	Surface(const shared_ptr<hittable_list>& triangle_array, const shared_ptr<Material>& material) : material(material)
	{
		triangles = make_shared<bvh_node>(*triangle_array);
		bbox = triangles->bounding_box();
	}

	bool hit(const Ray& r, interval ray_t, shared_ptr<hit_record>& rec) const override
	{
		return triangles->hit(r, ray_t, rec);
	}

	aabb bounding_box() const override
	{
		return bbox;
	}
private:
	shared_ptr<bvh_node> triangles;
	shared_ptr<Material> material;
	aabb bbox;
};

#endif

