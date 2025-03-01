#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "benchmark.h"
#include "hittable.h"
#include <vector>

using std::vector;
using std::make_shared;
using std::shared_ptr;

class hittable_list 
{
public:
    vector<shared_ptr<hittable>> objects;
    benchmark chrono;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    bool intersect(const ray& r, interval ray_t, shared_ptr<hit_record>& rec) const
    {
        shared_ptr<hit_record> temp_rec;
        bool hit_anything = false;
		auto closest_object_so_far = ray_t.max;

        for (const auto& object : objects) 
        {
            if (object->hit(r, interval(ray_t.min, closest_object_so_far), temp_rec))
            {
                hit_anything = true;
                closest_object_so_far = temp_rec->t;
				temp_rec->object = object;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};

#endif
