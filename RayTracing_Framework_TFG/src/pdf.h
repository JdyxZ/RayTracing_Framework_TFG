#ifndef PDF_H
#define PDF_H


class PDF // Probability Distribution Function (PDF)
{ 
public:
    virtual ~PDF() {}

    virtual double value(const vec3& direction) const = 0;
    virtual vec3 generate() const = 0;
};

class uniform_sphere_pdf : public PDF 
{
public:
    uniform_sphere_pdf() {}

    double value(const vec3& direction) const override 
    {
        return 1 / (4 * pi);
    }

    vec3 generate() const override 
    {
        return random_unit_vector();
    }
};

class cosine_hemisphere_pdf : public PDF 
{
public:
    cosine_hemisphere_pdf(const vec3& normal) : uvw(normal) {} // Generate a orthonormal basis of the hit point surface normal

    double value(const vec3& direction) const override 
    {
        auto cosine_theta = dot(unit_vector(direction), uvw.w());
        return std::fmax(0, cosine_theta / pi);
    }

    vec3 generate() const override 
    {
		// Generate a random cosine-weighted hemisphere direction
        vec3 scatter_direction = random_cosine_hemisphere_direction();

		// Intercept degenerate scatter direction (if the direction is near zero, scatter along the normal)
        // if (scatter_direction.near_zero())
            // scatter_direction = uvw.w(); 

		// Transform the scatter direction to the uvw space and normalize it
        scatter_direction = uvw.transform(scatter_direction);
		scatter_direction.normalize();
        return scatter_direction;
    }

private:
    ONB uvw;
};

class hittable_pdf : public PDF
{
public:
    hittable_pdf(shared_ptr<Hittable> object, const point3& hit_point)
        : object(object), hit_point(hit_point)
    {}

    double value(const vec3& direction) const override
    {
        return object->pdf_value(hit_point, direction);
    }
    vec3 generate() const override
    {
        return object->random_scattering_ray(hit_point);
    }
private:
    shared_ptr<Hittable> object;
    point3 hit_point;
};

class hittables_pdf : public PDF 
{
public:
    hittables_pdf(const vector<shared_ptr<Hittable>>& hittables, const point3& hit_point)
        : hittables(hittables), hit_point(hit_point)
    {}

    double value(const vec3& scattering_direction) const override 
    {
        auto size = hittables.size();
        auto weight = 1.0 / size;
        auto sum = 0.0;

        for (int i = 0; i < size; i++)
        {
            const auto& object = hittables[i];
            sum += weight * object->pdf_value(hit_point, scattering_direction);
        }

        return sum;
    }

    vec3 generate() const override 
    {
        auto size = int(hittables.size());
        auto random_object_index = random_int(0, size - 1);
        return hittables[random_object_index]->random_scattering_ray(hit_point);
    }

private:
    const vector<shared_ptr<Hittable>>& hittables;
    point3 hit_point;
};

class mixture_pdf : public PDF 
{
public:
    mixture_pdf(shared_ptr<PDF> p0, shared_ptr<PDF> p1)
    {
        p[0] = p0;
        p[1] = p1;
    }

    double value(const vec3& direction) const override 
    {
        return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
    }

    vec3 generate() const override 
    {
        if (random_double() < 0.5)
            return p[0]->generate();
        else
            return p[1]->generate();
    }

private:
    shared_ptr<PDF> p[2];
};

#endif
