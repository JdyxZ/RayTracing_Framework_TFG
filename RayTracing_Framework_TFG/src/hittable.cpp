// Headers
#include "core.hpp"
#include "hittable.hpp"
#include "utilities.hpp"
#include "quaternion.hpp"

void hit_record::determine_normal_direction(const vec3& ray_direction, const vec3& outward_normal)
{
    // NOTE: The parameter `outward_normal` is assumed to have unit length.
    front_face = dot(ray_direction, outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
}

sphere_hit_record::sphere_hit_record()
{
    type = SPHERE;
}

triangle_hit_record::triangle_hit_record() : bc(nullopt)
{
    type = TRIANGLE;
}

quad_hit_record::quad_hit_record()
{
    type = QUAD;
}

Hittable::Hittable()
{

}

double Hittable::pdf_value(const point3& hit_point, const vec3& scattering_direction) const
{
    return 0.0;
}

vec3 Hittable::random_scattering_ray(const point3& hit_point) const
{
    return vec3(1, 0, 0);
}

const PRIMITIVE Hittable::get_type() const
{
    return type;
}

const bool Hittable::has_pdf() const
{
    return pdf;
}

void Hittable::translate(const vec3& translation)
{
    _transform->set_translation(translation);
    _transform->cache_model();
    apply_transform = true;
}

void Hittable::rotate(const vec3& axis, const double angle)
{
    Quaternion rotation_quaterion = Quaternion(axis, angle);
    _transform->set_rotation(rotation_quaterion);
    _transform->cache_model();
    apply_transform = true;
}

void Hittable::scale(const vec3& scaling)
{
    _transform->set_scaling(scaling);
    _transform->cache_model();
    apply_transform = true;
}

void Hittable::transform(const Transform& transform)
{
    this->_transform = make_shared<Transform>(transform);
    this->_transform->cache_model();
    apply_transform = true;
}

shared_ptr<Transform> Hittable::get_transform()
{
    return _transform;
}
