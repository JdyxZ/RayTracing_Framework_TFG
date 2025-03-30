// Headers
#include "core.hpp"
#include "mesh.hpp"
#include "hittable_list.hpp"
#include "bvh.hpp"
#include "surface.hpp"
#include "interval.hpp"
#include "chrono.hpp"
#include "aabb.hpp"

Mesh::Mesh(const string& name, const shared_ptr<hittable_list>& surfaces, const vector<string>& material_names, const vector<string>& texture_names)
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
		*mesh_bvh_chrono += *surface->bvh_chrono();
	}
}

bool Mesh::hit(const shared_ptr<Ray>& r, Interval ray_t, shared_ptr<hit_record>& rec) const
{
	return surfaces->hit(r, ray_t, rec);
}

shared_ptr<AABB> Mesh::bounding_box() const
{ 
	return bbox; 
}

const shared_ptr<Chrono> Mesh::bvh_chrono() const
{
	return mesh_bvh_chrono; 
}

const string& Mesh::name() const
{ 
	return _name; 
}

const int& Mesh::num_triangles() const
{ 
	return _num_triangles; 
}

const int& Mesh::num_surfaces() const
{ 
	return _num_surfaces; 
}

const vector<string>& Mesh::material_names()
{ 
	return _material_names; 
}

const vector<string>& Mesh::texture_names()
{ 
	return _texture_names; 
}

