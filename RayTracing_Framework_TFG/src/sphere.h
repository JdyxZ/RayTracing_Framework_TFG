#ifndef SPHERE_H
#define SPHERE_H

class Sphere : public Hittable
{
public:

    // Stationary sphere
    Sphere(const point3& static_center, const double radius, shared_ptr<Material> material) : radius(std::fmax(0, radius)), material(material)
    {
        type = SPHERE;

        vec3 origin = static_center;
        vec3 direction = vec3(0);

        center = motion_vector(origin, direction);

        vec3 radius_vector = vec3(radius, radius, radius);
        bbox = aabb(static_center - radius_vector, static_center + radius_vector);
    }

    // Moving sphere
    Sphere(const point3& start_center, const point3& end_center, const double radius, shared_ptr<Material> material) : radius(std::fmax(0, radius)), material(material)
    {
        vec3 origin = start_center;
        vec3 direction = end_center - start_center;

        center = motion_vector(origin, direction);

        vec3 radius_vector = vec3(radius, radius, radius);
        aabb box1(center.at(0) - radius_vector, center.at(0) + radius_vector);
        aabb box2(center.at(1) - radius_vector, center.at(1) + radius_vector);
        bbox = aabb(box1, box2);
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
        auto sph_rec = make_shared<sphere_hit_record>();
        sph_rec->t = root;
        sph_rec->p = phit;
        sph_rec->material = material;
        sph_rec->determine_normal_direction(r.direction(), outward_normal);
        sph_rec->texture_coordinates = get_sphere_uv(outward_normal);
        sph_rec->type = type;


        
        // Polymorphic assignment
        rec = sph_rec;

        return true;
    }

    aabb bounding_box() const override 
    { 
        return bbox; 
    }

private:
    motion_vector center;
    double radius;
    shared_ptr<Material> material;
    aabb bbox;

    static pair<double, double> get_sphere_uv(const point3& p)
    {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.

        auto theta = std::acos(-p.y());
        auto phi = std::atan2(-p.z(), p.x()) + pi;

        return make_pair(phi / (2 * pi), theta / pi);
    }
};

#endif
