﻿// Headers
#include "core.hpp"
#include "camera.hpp"
#include "utilities.hpp"
#include "scene.hpp"
#include "image_writer.hpp"
#include "chrono.hpp"
#include "ray.hpp"
#include "color.hpp"
#include "hittable.hpp"
#include "interval.hpp"
#include "material.hpp"
#include "pdf.hpp"
#include "triangle.hpp"

Camera::Camera() 
{
    render_chrono = make_shared<Chrono>();
}

void Camera::initialize(const Scene& scene, const ImageWriter& image)
{
    // Determine viewport dimensions
    auto theta = degrees_to_radians(vertical_fov);
    auto h = std::tan(theta / 2);
    viewport_height = 2 * h * focus_distance;
    viewport_width = viewport_height * (double(image.width) / image.height);

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
    view = unit_vector(lookfrom - lookat);
    side = unit_vector(cross(world_up, view));
    up = cross(view, side);

    // Calculate the vectors across the horizontal, down the vertical and orthogonal the viewport edges respectively.
    vec3 viewport_u = viewport_width * side;
    vec3 viewport_v = viewport_height * -up;
    vec3 viewport_w = focus_distance * view;

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u = viewport_u / image.width;
    pixel_delta_v = viewport_v / image.height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = lookfrom - viewport_w - viewport_u / 2 - viewport_v / 2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Calculate the camera defocus disk basis vectors.
    auto defocus_radius = std::tan(degrees_to_radians(defocus_angle / 2));
    defocus_disk_u = side * defocus_radius * focus_distance;
    defocus_disk_v = up * defocus_radius * focus_distance;

    // Calculate the square root of samples per pixel and its inverse for stratified sampling
    pixel_sample_sqrt = int(sqrt(scene.samples_per_pixel));
    pixel_sample_sqrt_inv = 1.0 / pixel_sample_sqrt;

    // Log info
    Logger::info("CAMERA", "Camera settings succesfully initialized.");
}

void Camera::render(Scene& scene, ImageWriter& image)
{
    // Log info
    Logger::info("CAMERA", "Rendering started.");

    // Calculate primary rays to cast
    primary_rays = image.height * image.width * pixel_sample_sqrt * pixel_sample_sqrt;

    // Start render chrono
    render_chrono->start();

    for (int pixel_row = 0; pixel_row < image.height; pixel_row++)
    {
        // Progress info
        std::clog << "\rScanlines remaining: " << (image.height - pixel_row) << ' ' << std::flush;

        for (int pixel_column = 0; pixel_column < image.width; pixel_column++)
        {
            // Final pixel color
            color pixel_color(0, 0, 0);

            // Sample points with stratified sampling for antialisasing
            for (int sample_row = 0; sample_row < pixel_sample_sqrt; sample_row++)
            {
                for (int sample_column = 0; sample_column < pixel_sample_sqrt; sample_column++)
                {
                    // Get ray sample around pixel location
                    auto sample_ray = get_ray_sample(pixel_row, pixel_column, sample_row, sample_column);

                    // Get pixel color of the sample point that ray sample points to
                    pixel_color += ray_color(sample_ray, scene.bounce_max_depth, scene);
                }
            }

            // Avarage samples
            pixel_color /= scene.samples_per_pixel;

            // Compute color
            tuple<int, int, int> RGB_color = compute_color(pixel_color);

            // Determine pixel position in image buffer
            int pixel_position = 3 * (image.width * pixel_row + pixel_column);

            // Save pixel color into image buffer (row-major order)
            image.write_pixel(pixel_position, RGB_color);
        }
    }

    // Progress info end line
    std::cout << std::endl;

    // End render chrono
    render_chrono->end();

    // Benchmark rays
    rays_casted = primary_rays + reflected_rays + refracted_rays;
    average_rays_per_second = rays_casted / render_chrono->elapsed_miliseconds();
}


const shared_ptr<Ray> Camera::get_ray_sample(int pixel_row, int pixel_column, int sample_row, int sample_column) const
{
    auto offset = sample_square_stratified(sample_row, sample_column, pixel_sample_sqrt_inv);

    auto pixel_sample = pixel00_loc
        + ((pixel_row + offset.y) * pixel_delta_v)
        + ((pixel_column + offset.x) * pixel_delta_u);

    auto ray_origin = (defocus_angle <= 0) ? lookfrom : defocus_disk_sample(lookfrom, defocus_disk_u, defocus_disk_v);
    auto ray_direction = pixel_sample - ray_origin;

    auto ray_time = random_double();

    return make_shared<Ray>(ray_origin, ray_direction, ray_time);
}

color Camera::ray_color(const shared_ptr<Ray>& sample_ray, int depth, const Scene& scene)
{
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0, 0, 0);

    // Intersection details and closest object hit by the ray
    shared_ptr<hit_record> rec;

    // Define ray intersection interval
    Interval ray_t(scene.min_hit_distance, infinity);

    // Sky hit
    if (!scene.intersect(sample_ray, ray_t, rec))
        return scene.sky_blend ? sky_blend(sample_ray) : scene.background;

    // Hit object type
    PRIMITIVE hit_object_type = rec->type;

    // Material intersection point colors
    color color_from_scatter;
    color color_from_emission = rec->material->emitted(sample_ray, rec);

    // Get scene objects with specific PDFs
    auto hittables_with_pdf = scene.hittables_with_pdf;

    // Scattering record for pdf and attenuation management
    scatter_record srec;

    // If the ray hits an object, calculate the color of the hit point
    switch (hit_object_type)
    {
    case TRIANGLE:
    {
        /*
        std::shared_ptr<triangle_hit_record> tri_rec = std::dynamic_pointer_cast<triangle_hit_record>(rec);

        if (tri_rec)
        {
            throw std::runtime_error("Error in hit record downcast to triangle hit record");
            return color(0, 0, 0);
        }

        auto c = barycentric_color_interpolation(tri_rec, t);

        if (c.has_value())
            return c.value();

        return 0.5 * (tri_rec->normal + WHITE);
        */
    }
    case QUAD:

    case SPHERE:
    {
        // If the ray does not scatter, it is emissive
        if (!rec->material->scatter(sample_ray, rec, srec))
            return color_from_emission;

        // Deal with specular materials apart from the rest (PDF skip)
        if (srec.is_specular)
        {
            if (depth - 1 > 0)
            {
                switch (srec.scatter_type)
                {
                case REFLECT: reflected_rays++; break;
                case REFRACT: refracted_rays++; break;
                }
            }

            return srec.attenuation * ray_color(srec.specular_ray, depth - 1, scene);
        }


        // Create the sampling PDF
        shared_ptr<PDF> sampling_pdf;

        if (hittables_with_pdf.empty())
        {
            // Material associated samplig PDF
            sampling_pdf = srec.pdf;
        }
        else
        {
            // Generate mixture of PDFs
            auto _hittables_pdf = make_shared<hittables_pdf>(hittables_with_pdf, rec->p);
            auto _mixture_pdf = make_shared<mixture_pdf>(_hittables_pdf, srec.pdf);
            sampling_pdf = _mixture_pdf;
        }

        // Generate random scatter ray using the sampling PDF
        vec3 surface_hit_point = rec->p;
        vec3 scatter_direction = sampling_pdf->generate();
        auto scattered = make_shared<Ray>(surface_hit_point, scatter_direction, sample_ray->time());

        // Get the weight of the generated scatter ray sample
        auto sampling_pdf_value = sampling_pdf->value(scatter_direction);

        // Get the material's associated scattering PDF
        auto scattering_pdf_value = rec->material->scattering_pdf_value(sample_ray, rec, scattered);

        // Ray bounce
        if (depth - 1 > 0)
        {
            switch (srec.scatter_type)
            {
            case REFLECT: reflected_rays++; break;
            case REFRACT: refracted_rays++; break;
            }
        }

        color sample_color = ray_color(scattered, depth - 1, scene);

        // Bidirectional Reflectance Distribution Function (BRDF)
        color_from_scatter = (srec.attenuation * scattering_pdf_value * sample_color) / sampling_pdf_value;

        // Combine scatter and emission colors
        return color_from_emission + color_from_scatter;
    }
    default: // Unknown hit
        return scene.sky_blend ? sky_blend(sample_ray) : scene.background;
    }
}

color Camera::sky_blend(const shared_ptr<Ray>& r) const
{
    vec3 unit_direction = unit_vector(r->direction());

    auto a = 0.5 * (unit_direction.y + 1.0);
    color start_color = WHITE;
    color end_color = SKY_BLUE;

    return lerp(a, start_color, end_color);
}

optional<color> Camera::barycentric_color_interpolation(const shared_ptr<triangle_hit_record>& rec, Triangle* t) const
{
    if (!rec->bc.has_value() || !t->has_vertex_colors())
        return nullopt;

    // Barycentric coordinates
    barycentric_coordinates bc = rec->bc.value();
    double u = bc.u;
    double v = bc.v;
    double w = bc.w;

    // Vertex colors
    vec3 color_u = t->A.color.value();
    vec3 color_v = t->B.color.value();
    vec3 color_w = t->C.color.value();

    color c = u * color_u + v * color_v + w * color_w;

    return c;
}