#ifndef MESH_H
#define MESH_H

class Mesh : public Hittable
{
public:

	Mesh(const string& name, const shared_ptr<hittable_list>& surfaces, const vector<string>& material_names, const vector<string>& texture_names)
		: _name(name), _material_names(material_names), _texture_names(texture_names)
	{
		type = MESH;

		_num_surfaces = int(surfaces->size());
		this->surfaces = make_shared<bvh_node>(*surfaces);
		bbox = this->surfaces->bounding_box();
		mesh_bvh_chrono = this->surfaces->bvh_chrono();

		for (auto object : surfaces->objects)
		{
			auto surface = std::dynamic_pointer_cast<Surface>(object);
			_num_triangles += surface->num_triangles();
			mesh_bvh_chrono += surface->bvh_chrono();
		}
	}

	bool hit(const Ray& r, interval ray_t, shared_ptr<hit_record>& rec) const override
	{
		return surfaces->hit(r, ray_t, rec);
	}

	aabb bounding_box() const override { return bbox; }
	const Chrono& bvh_chrono() const { return mesh_bvh_chrono; }
	const string& name() const { return _name; }
	const int& num_triangles() const { return _num_triangles; }
	const int& num_surfaces() const { return _num_surfaces;  }
	const vector<string>& material_names() { return _material_names; }
	const vector<string>& texture_names() {return _texture_names; }


private:
	string _name = "error.obj";
	int _num_triangles = 0;
	int _num_surfaces = 0;
	vector<string> _material_names;
	vector<string> _texture_names;
	shared_ptr<bvh_node> surfaces;
	Chrono mesh_bvh_chrono;
	aabb bbox;
};

#endif

