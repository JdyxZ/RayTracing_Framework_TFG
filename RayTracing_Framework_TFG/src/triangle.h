#ifndef TRIANGLE_H
#define TRIANGLE_H

bool CULLING = false;

struct vertex 
{
	// Compulsory vertex attributes
    vec3 position;

	// Optional vertex attributes
    optional<vec3> normal, color;
    optional<pair<double, double>> uv;
};

class Triangle : public Hittable
{
public:
    vertex A, B, C;

    Triangle(vertex A, vertex B, vertex C, shared_ptr<Material> material)
		: A(A), B(B), C(C), material(material)
    {
        type = TRIANGLE;

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
        tri_rec->material = material;
        tri_rec->texture_coordinates = interpolate_texture_coordinates(u, v, w);
        tri_rec->type = type;
        tri_rec->bc = { u, v, w };
        tri_rec->determine_normal_direction(r.direction(), N);

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

private:
    vec3 AB, AC, N;
    shared_ptr<Material> material;
    aabb bbox;

    pair<double, double> interpolate_texture_coordinates(double u, double v, double w) const
    {
        // If any vertex is missing UVs, return a default (0,0) coordinate
        if (!A.uv.has_value() || !B.uv.has_value() || !C.uv.has_value())
            return { 0.0, 0.0 };

        // Retrieve the UVs from the vertices
        auto [uA, vA] = A.uv.value();
        auto [uB, vB] = B.uv.value();
        auto [uC, vC] = C.uv.value();

        // Interpolate UV coordinates using barycentric coordinates
        double u_interp = w * uA + u * uB + v * uC;
        double v_interp = w * vA + u * vB + v * vC;

        return { u_interp, v_interp };
    }
};

#endif 