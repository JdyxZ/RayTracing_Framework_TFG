#ifndef HITTABLE_TRANSFORM_H
#define HITTABLE_TRANSFORM_H

class translate : public Hittable 
{
public:
    translate(shared_ptr<Hittable> object, const vec3& offset) : object(object), offset(offset)
    {
        bbox = compute_translated_bbox();
    }

    bool hit(const Ray& r, interval ray_t, shared_ptr<hit_record>& rec) const override
    {
        // Move the ray backwards by the offset
        Ray translated_ray(r.origin() - offset, r.direction(), r.time());

        // Determine whether an intersection exists along the offset ray (and if so, where)
        if (!object->hit(translated_ray, ray_t, rec))
            return false;

        // Move the intersection point forwards by the offset
        rec->p += offset;

        return true;
    }

    aabb bounding_box() const override
    {
        return bbox;
    }

private:
    shared_ptr<Hittable> object;
    vec3 offset;
    aabb bbox;

    aabb compute_translated_bbox()
    {
        return object->bounding_box() + offset;
    }
};

class rotate : public Hittable
{
public:
    rotate(shared_ptr<Hittable> object, vec3 axis, double angle) : object(object)
    {
        // Transform degress to radians
        auto radians = degrees_to_radians(angle);

        // Normalize axis for unit quaternion
        axis.normalize();

        // Compute the rotation quaternion and its conjugate
        rotation_quat = Quaternion(axis, radians);
        inverse_rotation_quat = rotation_quat.conjugate();

        // Rotate the bounding box
        bbox = compute_rotated_bbox();
    }

    bool hit(const Ray& r, interval ray_t, shared_ptr<hit_record>& rec) const override
    {
        // Transform the ray from world space to object space using rotation quaternion
        vec3 rotated_origin = rotate3D(rotation_quat, r.origin(), inverse_rotation_quat);
        vec3 rotated_direction = rotate3D(rotation_quat, r.direction(), inverse_rotation_quat);
        Ray rotated_ray(rotated_origin, rotated_direction, r.time());

        // Determine if the rotated ray hits the object
        if (!object->hit(rotated_ray, ray_t, rec))
            return false;

        // Transform the intersection point and normal back to world space
        rec->p = rotate3D(inverse_rotation_quat, rec->p, rotation_quat);
        rec->normal = rotate3D(inverse_rotation_quat, rec->normal, rotation_quat);

        return true;
    }

    aabb bounding_box() const override 
    {
        return bbox;
    }

private:
    shared_ptr<Hittable> object;
    Quaternion rotation_quat;
    Quaternion inverse_rotation_quat;
    aabb bbox;

    aabb compute_rotated_bbox() const
    {
        // Get the original bounding box
        aabb original_bbox = object->bounding_box();

        // Iterate and rotate the 8 corner points of the AABB and find the new min and max
        point3 min(infinity, infinity, infinity);
        point3 max(-infinity, -infinity, -infinity);

        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    double x = i * original_bbox.x.max + (1 - i) * original_bbox.x.min;
                    double y = j * original_bbox.y.max + (1 - j) * original_bbox.y.min;
                    double z = k * original_bbox.z.max + (1 - k) * original_bbox.z.min;

                    // Rotate the corner point
                    vec3 rotated_corner = rotate3D(rotation_quat, point3(x, y, z), inverse_rotation_quat);
                    
                    // Update the new bounding box
                    for (int c = 0; c < 3; c++) 
                    {
                        min[c] = std::fmin(min[c], rotated_corner[c]);
                        max[c] = std::fmax(max[c], rotated_corner[c]);
                    }
                }
            }
        }
        
        return aabb(min, max);
    }
};

class scale : public Hittable
{
public:
    scale(shared_ptr<Hittable> object, const vec3& scale_factor) : object(object), scale_factor(scale_factor), inverse_scale(1.0 / scale_factor)
    {
        bbox = compute_scaled_bbox();
    }

    bool hit(const Ray& r, interval ray_t, shared_ptr<hit_record>& rec) const override
    {
        // Scale the ray into object space
        vec3 scaled_origin = r.origin() * inverse_scale;
        vec3 scaled_direction = r.direction() * inverse_scale;
        Ray scaled_ray(scaled_origin, scaled_direction, r.time());

        // Check if the scaled ray hits the object
        if (!object->hit(scaled_ray, ray_t, rec))
            return false;

        // Transform intersection point back to world space
        rec->p *= scale_factor;

        // Correct the normal using inverse transpose scaling
        rec->normal = (rec->normal / scale_factor).normalize();

        return true;
    }

    aabb bounding_box() const override 
    {
        return bbox;
    }

private:
    shared_ptr<Hittable> object;
    vec3 scale_factor;
    vec3 inverse_scale;
    aabb bbox;

    aabb compute_scaled_bbox() const
    {
        return object->bounding_box() * scale_factor;
    }
};

#endif
