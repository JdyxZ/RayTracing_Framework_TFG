#pragma once

// Headers
#include "interval.hpp"
#include "vec3.hpp"

// Forward declarations
class Ray;

class aabb
{
public:
    interval x, y, z;
    static const aabb empty, universe;

    aabb(); 
    aabb(const interval& x, const interval& y, const interval& z);
    aabb(const point3& a, const point3& b);
    aabb(const point3& a, const point3& b, const point3& c);
    aabb(const aabb& box0, const aabb& box1);
    aabb(const shared_ptr<aabb>& box0, const shared_ptr<aabb>& box1);

    const interval& axis_interval(int n) const;
    int longest_axis() const; // Returns the index of the longest axis of the bounding box.
    bool hit(const shared_ptr<Ray>& r, interval ray_t) const;

private:
    double delta = 0.0001;
    void pad_to_minimums();
};

// Operator overloads
aabb operator+(const aabb& bbox, const vec3& offset);
aabb operator+(const vec3& offset, const aabb& bbox);
aabb operator*(const aabb& bbox, const vec3& offset);
aabb operator*(const vec3& offset, const aabb& bbox);

shared_ptr<aabb> operator+(const shared_ptr<aabb>& bbox, const vec3& offset);
shared_ptr<aabb> operator+(const vec3& offset, const shared_ptr<aabb>& bbox);
shared_ptr<aabb> operator*(const shared_ptr<aabb>& bbox, const vec3& offset);
shared_ptr<aabb> operator*(const vec3& offset, const shared_ptr<aabb>& bbox);

