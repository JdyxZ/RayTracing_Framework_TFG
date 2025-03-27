#ifndef BVH_H
#define BVH_H

class bvh_node : public Hittable 
{
public:

    // Hierarchy specs
    int depth;
    int nodes;

    // Creates an implicit copy of the hittable list, which we will modify. 
    // The lifetime of the copied list only extends until this constructor exits.
    bvh_node(hittable_list list)
    {
        // init vars
        type = BVH_NODE;
        chrono = Chrono();

        chrono.start();

        *this = bvh_node(list.objects, 0, list.objects.size());

        chrono.end();
    }

    bvh_node(vector<shared_ptr<Hittable>>& objects, size_t start, size_t end) 
    {
        // Build the bounding box of the span of source objects.
        bbox = aabb::empty;
        for (size_t object_index = start; object_index < end; object_index++)
            bbox = aabb(bbox, objects[object_index]->bounding_box());

        int axis = bbox.longest_axis();

        auto comparator = (axis == 0) ? box_x_compare
                        : (axis == 1) ? box_y_compare
                        : box_z_compare;

        size_t object_span = end - start;

        if (object_span == 1) 
        {
            left = right = objects[start];
            depth = 0;
            nodes = 1; // Same object for left and and right leaf
        }
        else if (object_span == 2) 
        {
            left = objects[start];
            right = objects[start + 1];
            depth = 0;
            nodes = 1; 
        }
        else 
        {
            // Sort the objects based on the chosen axis
            std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

            // Create nodes
            auto mid = start + object_span / 2;
            auto left_node = make_shared<bvh_node>(objects, start, mid);
            auto right_node = make_shared<bvh_node>(objects, mid, end);

            // Update depth and nodes based on the children
            depth = std::max(left_node->depth, right_node->depth) + 1;
            nodes = 1 + left_node->nodes + right_node->nodes;

            // Assign nodes to hittable pointers
            left = left_node;
            right = right_node;
        }
    }

    bool hit(const Ray& r, interval ray_t, shared_ptr<hit_record>& rec) const override
    {
        if (!bbox.hit(r, ray_t))
            return false;

        bool hit_left = left->hit(r, ray_t, rec);
        bool hit_right = right->hit(r, interval(ray_t.min, hit_left ? rec->t : ray_t.max), rec);

        return hit_left || hit_right;
    }

    aabb bounding_box() const override 
    { 
        return bbox; 
    }

    const Chrono& bvh_chrono() const
    {
        return chrono;
    }

private:
    shared_ptr<Hittable> left;
    shared_ptr<Hittable> right;
    aabb bbox;
    Chrono chrono;

    static bool box_compare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b, int axis_index)
    {
        auto a_axis_interval = a->bounding_box().axis_interval(axis_index);
        auto b_axis_interval = b->bounding_box().axis_interval(axis_index);
        return a_axis_interval.min < b_axis_interval.min;
    }

    static bool box_x_compare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b)
    {
        return box_compare(a, b, 0);
    }

    static bool box_y_compare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b)
    {
        return box_compare(a, b, 1);
    }

    static bool box_z_compare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b)
    {
        return box_compare(a, b, 2);
    }
};

#endif

