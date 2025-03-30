// Headers
#include "core.hpp"
#include "aabb.hpp"
#include "ray.hpp"

aabb::aabb() 
{
    // The default AABB is empty, since intervals are empty by default.
} 

aabb::aabb(const interval& x, const interval& y, const interval& z) : x(x), y(y), z(z)
{
    pad_to_minimums();
}

aabb::aabb(const point3& a, const point3& b)
{
    // Treat the two points a and b as extrema for the bounding box, so we don't require a particular minimum/maximum coordinate order.
    x = interval(std::min(a[0], b[0]), std::max(a[0], b[0]));
    y = interval(std::min(a[1], b[1]), std::max(a[1], b[1]));
    z = interval(std::min(a[2], b[2]), std::max(a[2], b[2]));

    pad_to_minimums();
}

aabb::aabb(const point3& a, const point3& b, const point3& c)
{
    x = interval(std::min({ a[0], b[0], c[0] }), std::max({ a[0], b[0], c[0] }));
    y = interval(std::min({ a[1], b[1], c[1] }), std::max({ a[1], b[1], c[1] }));
    z = interval(std::min({ a[2], b[2], c[2] }), std::max({ a[2], b[2], c[2] }));

    pad_to_minimums();
}


aabb::aabb(const aabb& box0, const aabb& box1)
{
    // The interval contructor method automatically orders the interval values
    x = interval(box0.x, box1.x);
    y = interval(box0.y, box1.y);
    z = interval(box0.z, box1.z);
}

aabb::aabb(const shared_ptr<aabb>& box0, const shared_ptr<aabb>& box1) : aabb(*box0, *box1) {}

const interval& aabb::axis_interval(int n) const
{
    if (n == 1) return y;
    if (n == 2) return z;
    return x;
}

int aabb::longest_axis() const
{
    if (x.size() > y.size())
        return x.size() > z.size() ? 0 : 2;
    else
        return y.size() > z.size() ? 1 : 2;
}

bool aabb::hit(const shared_ptr<Ray>& r, interval ray_t) const
{
    // ** Slab method ** //
    const point3& ray_orig = r->origin();
    const vec3& ray_dir = r->direction();

    for (int axis = 0; axis < 3; axis++)
    {
        const interval& ax = axis_interval(axis);

        // Calculate ray intersection with current axis interval extremes
        const double adinv = 1.0 / ray_dir[axis];
        auto t0 = (ax.min - ray_orig[axis]) * adinv;
        auto t1 = (ax.max - ray_orig[axis]) * adinv;

        if (t0 < t1)
        {
            if (t0 > ray_t.min) ray_t.min = t0;
            if (t1 < ray_t.max) ray_t.max = t1;
        }
        else
        {
            if (t1 > ray_t.min) ray_t.min = t1;
            if (t0 < ray_t.max) ray_t.max = t0;
        }

        if (ray_t.max <= ray_t.min)
            return false;
    }

    return true;
}


void aabb::pad_to_minimums()
{
    // Adjust the AABB so that no side is narrower than some delta, padding if necessary.
    if (x.size() < delta) x = x.expand(delta);
    if (y.size() < delta) y = y.expand(delta);
    if (z.size() < delta) z = z.expand(delta);
}

aabb operator+(const aabb& bbox, const vec3& offset) 
{
    return aabb(bbox.x + offset.x(), bbox.y + offset.y(), bbox.z + offset.z());
}

aabb operator+(const vec3& offset, const aabb& bbox) 
{
    return bbox + offset;
}

aabb operator*(const aabb& bbox, const vec3& offset) 
{
    return aabb(bbox.x * offset.x(), bbox.y * offset.y(), bbox.z * offset.z());
}

aabb operator*(const vec3& offset, const aabb& bbox) 
{
    return bbox * offset;
}

shared_ptr<aabb> operator+(const shared_ptr<aabb>& bbox, const vec3& offset)
{
    return make_shared<aabb>(*bbox + offset);
}

shared_ptr<aabb> operator+(const vec3& offset, const shared_ptr<aabb>& bbox)
{
    return bbox + offset;
}

shared_ptr<aabb> operator*(const shared_ptr<aabb>& bbox, const vec3& offset)
{
    return make_shared<aabb>(*bbox * offset);
}

shared_ptr<aabb> operator*(const vec3& offset, const shared_ptr<aabb>& bbox)
{
    return bbox * offset;
}

// Static members
const aabb aabb::empty = aabb(interval::empty, interval::empty, interval::empty);
const aabb aabb::universe = aabb(interval::universe, interval::universe, interval::universe);