#ifndef TRIANGLE_H
#define TRIANGLE_H

bool CULLING = false;

struct vertex 
{
	// Compulsory vertex attributes
    vec3 position;

	// Optional vertex attributes
    optional<vec3> normal, color;
};

class Triangle : public Hittable
{
public:
    vertex A, B, C;
	shared_ptr<material> mat;

    Triangle(vertex A, vertex B, vertex C, shared_ptr<material> mat)
		: A(A), B(B), C(C), mat(mat)
    {
        AB = B.position - A.position;
        AC = C.position - A.position;
		N = cross(AB, AC).normalize();
        
        bbox = aabb(A.position, B.position, C.position);
    }

    bool hit(const Ray& r, interval ray_t, shared_ptr<hit_record>& rec) const override
    {
        // Calculate P vector and determinant
        vec3 P = cross(r.direction(), AC);
        double det = dot(AB, P);

        // If the determinant is negative, the triangle is back-facing.
		// If the determinant is close to 0, the ray misses the triangle (parallel).
		if (CULLING)
		{
			if (det < kEpsilon) return false;
		}
		else
		{
			if (fabs(det) < kEpsilon) return false;
		}

        // Invert determinant
        double invDet = 1 / det;

        // Get barycentric cordinate u and check if the ray hits inside the u-edge
        vec3 T = r.origin() - A.position;
        double u = dot(T, P) * invDet;
        if (u < 0 || u > 1) return false;

        // Get barycentric cordinate v and check if the ray hits inside the v-edge
        vec3 Q = cross(T, AB);
        double v = dot(r.direction(),Q) * invDet;
        if (v < 0 || u + v > 1) return false;

        // Get barycentric cordinate w
		double w = 1 - u - v;

		// Get value t of the ray
        double t = dot(AC, Q) * invDet;

		// Check if the intersection is within the valid range (check if the ray hits the triangle from behind or front)
        if (!ray_t.surrounds(t)) return false;

        // Hit record
        auto tri_rec = make_shared<triangle_hit_record>(); 
        tri_rec->t = t;
        tri_rec->p = r.at(t);
        tri_rec->determine_normal_direction(r.direction(), N);
        tri_rec->bc = { u, v, w };

		// Polymorphic assignment
        rec = tri_rec;

        return true;
    }

	bool has_vertex_colors() const
	{
		return A.color.has_value() && B.color.has_value() && C.color.has_value();
	}

	bool has_vertex_normals() const
	{
		return A.normal.has_value() && B.normal.has_value() && C.normal.has_value();
	}

    aabb bounding_box() const override 
    { 
        return bbox; 
    }

    const PRIMITIVE get_type() const override
    {
        return SPHERE;
    }

private:
    vec3 AB, AC, N;
    aabb bbox;
};

#endif