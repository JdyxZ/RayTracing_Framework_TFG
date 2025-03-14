#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

class hittable_list
{
public:
    // Primitives
    vector<shared_ptr<Hittable>> objects;

    void add(shared_ptr<Hittable> object)
    {
        objects.push_back(object);
    }

    void add(shared_ptr<hittable_list> list)
    {
        objects.insert(objects.end(), list->objects.begin(), list->objects.end());
    }

    void clear()
    {
        objects.clear();
    }

    bool intersect(const Ray& r, interval ray_t, shared_ptr<hit_record>& rec) const
    {
        shared_ptr<hit_record> temp_rec = make_shared<hit_record>();
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
};

#endif