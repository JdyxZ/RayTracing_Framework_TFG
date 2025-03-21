#ifndef MESH_H
#define MESH_H

class Mesh : public Hittable
{
public:
	Mesh() 
	{
		bbox = aabb::empty;
	}

	void add(const shared_ptr<Surface>& surface)
	{
		surfaces->add(surface);
		bbox = aabb(bbox, surface->bounding_box());
	}

	bool hit(const Ray& r, interval ray_t, shared_ptr<hit_record>& rec) const override
	{
		if (!bbox.hit(r, ray_t))
			return false;

		return surfaces->intersect(r, ray_t, rec);
	}

	aabb bounding_box() const override
	{
		return bbox;
	}

private:
	shared_ptr<hittable_list> surfaces;
	aabb bbox;
};

#endif

