#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <vector>

using std::vector;
using std::make_shared;
using std::shared_ptr;

class hittable_list {
public:
    vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    bool intersect(const ray& r, interval ray_t, hit_record& rec, shared_ptr<hittable>& closest_object)
    {
        hit_record temp_rec;
        bool hit_anything = false;

        for (const auto& object : objects) 
        {
            if (object->hit(r, ray_t, temp_rec))
            {
                hit_anything = true;
                ray_t.max = temp_rec.t; // closest object so far
                rec = temp_rec;
                closest_object = object;
            }
        }

        return hit_anything;
    }
};

#endif
