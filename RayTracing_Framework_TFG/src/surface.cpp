// Headers 
#include "core.hpp"
#include "surface.hpp"
#include "hittable_list.hpp"
#include "bvh.hpp"
#include "aabb.hpp"

Surface::Surface() {}

Surface::Surface(const shared_ptr<hittable_list>& triangles, const shared_ptr<Material>& material) : material(material)
{
	_num_triangles = int(triangles->size());
	this->triangles = make_shared<bvh_node>(*triangles);
	bbox = this->triangles->bounding_box();
	surface_bvh_chrono = this->triangles->bvh_chrono();
}

bool Surface::hit(const shared_ptr<Ray>& r, Interval ray_t, shared_ptr<hit_record>& rec) const
{
	return triangles->hit(r, ray_t, rec);
}

shared_ptr<AABB> Surface::bounding_box() const
{
	return bbox;
}

const shared_ptr<Chrono> Surface::bvh_chrono() const
{
	return surface_bvh_chrono;
}

const int& Surface::num_triangles() const
{
	return _num_triangles;
}
