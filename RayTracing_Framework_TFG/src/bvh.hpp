﻿#pragma once

// Headers
#include "core.hpp"
#include "hittable.hpp"

// Forward declarations
class hittable_list;
struct Chrono;

class bvh_node : public Hittable 
{
public:
    // Hierarchy specs
    int depth;
    int nodes;

    // Creates an implicit copy of the hittable list, which we will modify. 
    // The lifetime of the copied list only extends until this constructor exits.
    bvh_node(hittable_list list);  

    bvh_node(vector<shared_ptr<Hittable>>& objects, size_t start, size_t end);

    bool hit(const shared_ptr<Ray>& r, Interval ray_t, shared_ptr<hit_record>& rec) const override;
    shared_ptr<AABB> bounding_box() const override;
    const shared_ptr<Chrono> bvh_chrono() const;

private:
    shared_ptr<Hittable> left;
    shared_ptr<Hittable> right;
    shared_ptr<AABB> bbox;
    shared_ptr<Chrono> chrono;

    static bool box_compare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b, int axis_index);
    static bool box_x_compare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b);
    static bool box_y_compare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b);
    static bool box_z_compare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b);
};



