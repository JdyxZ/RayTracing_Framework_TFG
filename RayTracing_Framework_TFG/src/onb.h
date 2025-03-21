#ifndef ONB_H
#define ONB_H

class ONB  // Orthonormal Basis (ONB)
{ 
public:
    ONB(const vec3& n)
    {
		// Assume n is a unit vector!!!
        vec3 a = (fabs(n.x()) > 0.9) ? y_axis : x_axis;

        vec3 w = n;
		vec3 v = cross(w, a);
        vec3 u = cross(w, v);

        axis[0] = u;
		axis[1] = v;
		axis[2] = w;
    }

    const vec3& u() const 
    { 
        return axis[0]; 
    }

    const vec3& v() const 
    { 
        return axis[1]; 
    }

    const vec3& w() const 
    { 
        return axis[2]; 
    }

    // Transform from basis coordinates to local space.
    vec3 transform(const vec3& v) const 
    {
        return (v[0] * axis[0]) + (v[1] * axis[1]) + (v[2] * axis[2]);
    }

private:
    vec3 axis[3];
};


#endif