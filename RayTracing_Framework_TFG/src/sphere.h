#ifndef SPHERE_H
#define SPHERE_H

class sphere : public hittable
{
public:
    shared_ptr<material> mat;

    sphere(const point3& center, double radius, shared_ptr<material> mat) : center(center), radius(std::fmax(0, radius)), mat(mat) {}

    bool hit(const ray& r, interval ray_t, shared_ptr<hit_record>& rec) const override
    {
        vec3 oc = center - r.origin();
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
        vec3 outward_normal = (phit - center) / radius;

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
    point3 center;
    double radius;
};

#endif
