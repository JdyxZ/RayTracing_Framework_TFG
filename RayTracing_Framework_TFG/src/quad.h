#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"

class Quad : public Hittable 
{
public:

    Quad(const point3& Q, const vec3& u, const vec3& v, shared_ptr<Material> material, bool pdf = false) : Q(Q), u(u), v(v), material(material)
    {
        type = QUAD;
        this->pdf = pdf;

        auto n = cross(u, v);
        normal = unit_vector(n);
        D = dot(normal, Q);
        w = n / dot(n, n);
        area = n.length();

        set_bounding_box();
    }

    virtual void set_bounding_box() 
    {
        // Compute the bounding box of all four vertices.
        auto bbox_diagonal1 = aabb(Q, Q + u + v);
        auto bbox_diagonal2 = aabb(Q + u, Q + v);
        bbox = aabb(bbox_diagonal1, bbox_diagonal2);
    }

    aabb bounding_box() const override { return bbox; }

    bool hit(const Ray& r, interval ray_t, shared_ptr<hit_record>& rec) const override
    {
        auto denom = dot(normal, r.direction());

        // No hit if the ray is parallel to the plane.
        if (std::fabs(denom) < kEpsilon)
            return false;

        // Calculate the ray intersection value
        auto t = (D - dot(normal, r.origin())) / denom;

        // Return false if the hit point parameter t is outside the ray interval.
        if (!ray_t.surrounds(t))
            return false;

        // Intersection point
        auto P = r.at(t);

        // Obtain intersection point's planar coordinates of the coordinate frame determined by the plane determined by Q, u and v
        vec3 phit = P - Q; // Intersection point's vector expressed in plane basis coordinates
        auto alpha = dot(w, cross(phit, v));
        auto beta = dot(w, cross(u, phit));

        // Check whether the intersection point is within the quad
        if (!interval::unitary.contains(alpha) || !interval::unitary.contains(beta))
            return false;

        // Hit record
        auto quad_rec = make_shared<quad_hit_record>();
        quad_rec->t = t;
        quad_rec->p = P;
        quad_rec->material = material;
        quad_rec->texture_coordinates = make_pair(alpha, beta);
        quad_rec->determine_normal_direction(r.direction(), normal);

        // Polymorphic assignment
        rec = quad_rec;

        return true;
    }

    double pdf_value(const point3& hit_point, const vec3& scattering_direction) const override 
    {
        shared_ptr<hit_record> rec;

        if (!this->hit(Ray(hit_point, scattering_direction), interval(0.001, infinity), rec))
            return 0;

        auto distance_squared = rec->t * rec->t * scattering_direction.length_squared(); // light_hit_point - origin = t * direction
		auto cosine = fabs(dot(scattering_direction, rec->normal) / scattering_direction.length()); // scattering direction is not normalized

        return distance_squared / (cosine * area);
    }

    vec3 random_scattering_ray(const point3& hit_point) const override 
    {
        auto p = Q + (random_double() * u) + (random_double() * v);
        return p - hit_point;
    }

private:
    point3 Q;
    double D;
    vec3 u, v, w, normal;
    double area;
    shared_ptr<Material> material;
    aabb bbox;
};

#endif