#ifndef SCENE_H
#define SCENE_H

#include "benchmark.h"
#include "hittable.h"
#include <vector>

using std::vector;
using std::make_shared;
using std::shared_ptr;

class Scene 
{
public:

    // Ray scattering settings
    int    bounce_max_depth = 10;       // Maximum number of ray bounces into scene
    double min_hit_distance = 0.001;    // Greatly solves shadow acne

    // Antialiasing and noise settings
    int    samples_per_pixel = 10;      // Count of random samples for each pixel

    // Scene primitives
    vector<shared_ptr<Hittable>> objects;

    // Rendering benchmark
    Benchmark chrono;

    Scene() {}
    Scene(shared_ptr<Hittable> object) { add(object); }

    aabb bounding_box() const 
    { 
        return bbox; 
    }

    void add(shared_ptr<Hittable> object) 
    {
        objects.push_back(object);
        bbox = aabb(bbox, object->bounding_box());
    }

    void clear() 
    { 
        objects.clear(); 
    }

    bool intersect(const Ray& r, interval ray_t, shared_ptr<hit_record>& rec) const
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
                rec = temp_rec;
            }
        }

        return hit_anything;
    }

private:
    aabb bbox;
};

using hittable_list = Scene;

#endif
