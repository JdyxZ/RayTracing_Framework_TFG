﻿// Headers
#include "core.hpp"
#include "transform.hpp"
#include "vec3.hpp"
#include "quaternion.hpp"
#include "matrix.hpp"

Transform::Transform()
{
	translation = make_shared<vec3>(0.0);
	rotation = make_shared<Quaternion>();
	scaling = make_shared<vec3>(1.0);
	recompute = true;
	model = nullptr;
}

Transform::Transform(const vec3& translation, const Quaternion& rotation, const vec3& scailing)
{
	this->translation = make_shared<vec3>(translation);
	this->rotation = make_shared<Quaternion>(rotation);
	this->scaling = make_shared<vec3>(scailing);
	recompute = true;
	model = nullptr;

	this->cache_model();
}

shared_ptr<Transform> Transform::inverse()
{
	shared_ptr<Transform> inverse_transform = make_shared<Transform>();

	inverse_transform->set_scaling
	(
			vec3
			(
			std::fabs(scaling->x) < kEpsilon ? 0.0 : 1.0 / scaling->x,
			std::fabs(scaling->y) < kEpsilon ? 0.0 : 1.0 / scaling->y,
			std::fabs(scaling->z) < kEpsilon ? 0.0 : 1.0 / scaling->z
			)
	);

	inverse_transform->set_rotation(rotation->conjugate());

	inverse_transform->set_translation(*inverse_transform->rotation * (*inverse_transform->scaling * (*translation * -1.0)));

	return inverse_transform;
}

void Transform::set_translation(const vec3& translation)
{
	this->translation = make_shared<vec3>(translation);
	recompute = true;
}

void Transform::set_rotation(const Quaternion& rotation)
{
	this->rotation = make_shared<Quaternion>(rotation);
	recompute = true;
}

void Transform::set_scaling(const vec3& scailing)
{
	this->scaling = make_shared<vec3>(scailing);
	recompute = true;
}

void Transform::set_recompute(bool recompute)
{
	this->recompute = recompute;
}

bool Transform::needs_recompute() const
{
	return recompute;
}

void Transform::cache_model()
{
	model = get_model();
}

shared_ptr<Matrix44> Transform::get_model()
{
	if (!recompute)
		return model;

	auto model = compute_model();
	recompute = false;

	return model;
}

shared_ptr<Matrix44> Transform::compute_model()
{
	// Get the center of the coordinate system 
	point3 local_center = *translation;

	// Rotate basis vectors of the coordinate system
	vec3 local_x_axis = *rotation * x_axis;
	vec3 local_y_axis = *rotation * y_axis;
	vec3 local_z_axis = *rotation * flipped_z_axis;

	// Scale basis vectors of the coordinate system
	local_x_axis *= scaling->x;
	local_y_axis *= scaling->y;
	local_z_axis *= scaling->z;

	// Create transformation matrix
	return make_shared<Matrix44>
	(
		local_x_axis.x, local_y_axis.x, local_z_axis.x, local_center.x,
		local_x_axis.y, local_y_axis.y, local_z_axis.y, local_center.y,
		local_x_axis.z, local_y_axis.z, local_z_axis.z, local_center.z,
		0.0, 0.0, 0.0, 1.0
	);
}

const shared_ptr<Matrix44> Transform::transform_matrix(const vec3& translation, vec3 axis, const double& angle, const vec3& scailing)
{
	auto radians = degrees_to_radians(angle);
	axis.normalize();

	Quaternion rotation = Quaternion(axis, radians);
	Transform transform = Transform(translation, rotation, scailing);
	
	auto model = transform.get_model();

	return model;
}
