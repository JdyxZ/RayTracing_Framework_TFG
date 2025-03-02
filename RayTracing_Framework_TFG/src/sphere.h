#ifndef SPHERE_H
#define SPHERE_H

class Sphere : public Hittable
{
public:
    shared_ptr<material> mat;

    // Stationary sphere
    Sphere(const point3& static_center, const double radius, shared_ptr<material> mat) : radius(std::fmax(0, radius)), mat(mat) 
    {
        vec3 origin = static_center;
        vec3 direction = vec3(0);

        center = motion_vector(origin, direction);
    }

    // Moving sphere
    Sphere(const point3& start_center, const point3& end_center, const double radius, shared_ptr<material> mat) : radius(std::fmax(0, radius)), mat(mat)
    {
        vec3 origin = start_center;
        vec3 direction = end_center - start_center;

        center = motion_vector(origin, direction);
    }

    bool hit(const Ray& r, interval ray_t, shared_ptr<hit_record>& rec) const override
    {
        point3 current_center = center.at(r.time());

        vec3 oc = current_center - r.origin();
        auto a = r.direction().length_squared();
		auto h = dot(r.direction(), oc); // h = -b/2
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = h * h - a * c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        vec3 phit = r.at(root);
        vec3 outward_normal = (phit - current_center) / radius;

        // Hit record
        auto sph_rec = std::make_shared<sphere_hit_record>();
        sph_rec->t = root;
        sph_rec->p = phit;
        sph_rec->determine_normal_direction(r.direction(), outward_normal);
        
        // Polymorphic assignment
        rec = sph_rec;

        return true;
    }

    const PRIMITIVE get_type() const override
    {
        return SPHERE;
    }

private:
    motion_vector center;
    double radius;
};

#endif
