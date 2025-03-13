#include "core.h"

// Operators
Quaternion Quaternion::operator*(const double scalar) const
{
    return Quaternion(w * scalar, i * scalar, j * scalar, k * scalar);
}

// Quaternion Scalar Division
Quaternion Quaternion::operator/(const double scalar) const
{
    return Quaternion(w / scalar, i / scalar, j / scalar, k / scalar);
}

Quaternion Quaternion::operator+(const Quaternion& q) const
{
    return Quaternion(w + q.w, i + q.i, j + q.j, k + q.k);
}

// Quaternion Subtraction
Quaternion Quaternion::operator-(const Quaternion& q) const
{
    return Quaternion(w - q.w, i - q.i, j - q.j, k - q.k);
}

Quaternion Quaternion::operator*(const Quaternion& q) const 
{
    return Quaternion(
        w * q.w - i * q.i - j * q.j - k * q.k,
        w * q.i + i * q.w + j * q.k - k * q.j,
        w * q.j - i * q.k + j * q.w + k * q.i,
        w * q.k + i * q.j - j * q.i + k * q.w
    );
}

// Useful functions
Quaternion Quaternion::conjugate() const 
{
    return Quaternion(w, -i, -j, -k);
}

void Quaternion::normalize() 
{
	// Unit quaterion rule (w^2 + i^2 + j^2 + k^2 = 1)
    double norm = sqrt(w * w + i * i + j * j + k * k);

    if (norm > 0.0) 
    {
        w /= norm;
        i /= norm;
        j /= norm;
        k /= norm;
    }
}

double Quaternion::dot(const Quaternion& q) const
{
    return w * q.w + i * q.i + j * q.j + k * q.k;
}

// Create a unit quaternion for a given rotation angle and axis
Quaternion rotation_quaternion(double angle, const vec3& axis) 
{
	// Rotation quaternion follows the formula: 
    // 
    // angle = 0
    // axis_quaterion = (0, x, y, z)​
	// rotation_quaterion = cos(θ/2) ​+ (xi + yj + zk) sin(θ/2) = (cos(θ/2), sin(θ/2)x, sin(θ/2)y, sin(θ/2)z)

    double half_angle = angle * 0.5;
    double sin_half = sin(half_angle);

    return Quaternion(
        cos(half_angle),
        axis.x() * sin_half,
        axis.y() * sin_half,
        axis.z() * sin_half
    );
}

vec3 rotate(const Quaternion& q, const vec3& v, const Quaternion& q_inv)
{
    // 3D rotation with quaternions follos the formula v' = q * v * q^-1 (rotated_vector_quaternion = rotation_quaternion * vector_quaterion * rotation_quaternion_conjugate)
    Quaternion q_v(0, v.x(), v.y(), v.z());
    Quaternion rotated = q * q_v * q_inv;
    return vec3(rotated.i, rotated.j, rotated.k);
}

// Quaternion SLERP (Smooth Rotation Interpolation)
Quaternion slerp(const Quaternion& q1, const Quaternion& q2, double t)
{
    double cos_theta = q1.dot(q2);

    // If angle is small, do linear interpolation
    if (cos_theta > 0.9995)
    {
        return (q1 * (1 - t) + q2 * t).normalize();
    }

    // Compute SLERP interpolation
    double theta = acos(cos_theta);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    double a = sin((1 - t) * theta) / sin_theta;
    double b = sin(t * theta) / sin_theta;

    return q1 * a + q2 * b;
}