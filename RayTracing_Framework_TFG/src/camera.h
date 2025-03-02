#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "image.h"

class camera {
public:

    // Camera settings
    double vertical_fov = 90;           // Vertical view angle (field of view)
    double defocus_angle = 0;           // Angle of the cone with apex at viewport center and base at camera center (defocus disk)
    double focus_distance = 10;         // Distance from camera lookfrom point to plane of perfect focus

    // Image settings
    IMAGE_FORMAT image_format = PNG;    // Image format
    int image_quality = 100;            // Only for JPG images
    int    image_width = 400;           // Rendered image width in pixel count
    double aspect_ratio = 16.0 / 9.0;   // Ratio of image width over height

	// Viewport settings
    double viewport_height;	            // Height of viewport
	double viewport_width;              // Width of viewport

	// Camera position and orientation
    point3 lookfrom = point3(0, 0, 0);  // Point camera is looking from
    point3 lookat = point3(0, 0, -1);   // Point camera is looking at
    vec3   world_up = vec3(0, 1, 0);    // Camera-relative "up" direction

    // Ray scattering settings
    int    bounce_max_depth = 10;       // Maximum number of ray bounces into scene
	double min_hit_distance = 0.001;    // Greatly solves shadow acne

	// Antialiasing settings
    int    samples_per_pixel = 10;      // Count of random samples for each pixel

    void initialize() 
    {
        // Init image
        image = new Image(image_width, image_height, aspect_ratio);

        // Determine viewport dimensions
        auto theta = degrees_to_radians(vertical_fov);
        auto h = std::tan(theta / 2);
        viewport_height = 2 * h * focus_distance;
        viewport_width = viewport_height * (double(image_width) / image_height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        view = unit_vector(lookfrom - lookat);
        side = unit_vector(cross(world_up, view));
        up = cross(view, side);

        // Calculate the vectors across the horizontal, down the vertical and orthogonal the viewport edges respectively.
        vec3 viewport_u = viewport_width * side;    
        vec3 viewport_v = viewport_height * -up;
		vec3 viewport_w = focus_distance * view;

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = lookfrom - viewport_w - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors.
        auto defocus_radius = std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = side * defocus_radius * focus_distance;
        defocus_disk_v = up * defocus_radius * focus_distance;
    }

    void render(hittable_list& scene)
    {
        // Start benchmark chrono
		scene.chrono.start();

        for (int pixel_row = 0; pixel_row < image_height; pixel_row++)
        {
            // Progress info
            std::clog << "\rScanlines remaining: " << (image_height - pixel_row) << ' ' << std::flush;

            for (int pixel_column = 0; pixel_column < image_width; pixel_column++)
            {
				// Final pixel color
                color pixel_color(0, 0, 0);
                
                // Sample points for antialisasing
                for (int sample = 0; sample < samples_per_pixel; sample++) 
                {
                    // Get ray sample around pixel location
                    ray r = get_ray_sample(pixel_row, pixel_column);

					// Get pixel color of the sample point that ray sample points to
                    pixel_color += ray_color(r, bounce_max_depth, scene);
                }

                // Avarage samples
				pixel_color /= samples_per_pixel;

				// Compute color
                tuple<int,int,int> RGB_color = compute_color(pixel_color);

				// Determine pixel position in image buffer
                int pixel_position = 3 * (image_width * pixel_row + pixel_column);

                // Save pixel color into image buffer (row-major order)
                image->write_pixel(pixel_position, RGB_color);
            }
        }

        // Progress info end line
        std::cout << std::endl;

        // End benchmark chrono
        scene.chrono.end();

        // Save image with desired format
        switch (image_format)
        {
        case PNG:
            image->savePNG();
            break;
        case JPG:
            image->saveJPG(100);
            break;
        }
    }

private:

    // Internal variables
    point3 pixel00_loc;         // Location of pixel 0, 0
    vec3   pixel_delta_u;       // Offset to pixel to the right
    vec3   pixel_delta_v;       // Offset to pixel below
    vec3   side, up, view;      // Camera frame basis vectors
    vec3   defocus_disk_u;      // Defocus disk horizontal radius
    vec3   defocus_disk_v;      // Defocus disk vertical radius

    // Image
    Image* image;
    int image_height;                       // Rendered image height                    

    // Construct a camera ray originating from the defocus disk and directed at randomly sampled point around the pixel location pixel_row, pixel_column.
    ray get_ray_sample(int pixel_row, int pixel_column) const
    {
        auto offset = sample_square();

        auto pixel_sample = pixel00_loc
            + ((pixel_row + offset.y()) * pixel_delta_v)
            + ((pixel_column + offset.x()) * pixel_delta_u);

        auto ray_origin = (defocus_angle <= 0) ? lookfrom : defocus_disk_sample(lookfrom, defocus_disk_u, defocus_disk_v);
        auto ray_direction = pixel_sample - ray_origin;

        auto ray_time = random_double();

        return ray(ray_origin, ray_direction, ray_time);
    }

    color ray_color(const ray& r, int depth, const hittable_list& scene) const
    {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return color(0, 0, 0);

        // Intersection details and closest object hit by the ray
        shared_ptr<hit_record> rec;

        // Define ray intersection interval
        interval ray_t(min_hit_distance, infinity);

        // Sky hit
        if (!scene.intersect(r, ray_t, rec))
            return sky_blend(r);

        // If the ray hits an object, calculate the color of the hit point
        if (auto s = dynamic_cast<sphere*>(rec->object.get()))
        {
            ray scattered;
            color attenuation;

            if (s->mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth - 1, scene);

            return color(0, 0, 0); 
        }
        else if (auto t = dynamic_cast<triangle*>(rec->object.get()))
        {
            ray scattered;
            color attenuation;

            if (t->mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth - 1, scene);

            return color(0, 0, 0);

            /*
            std::shared_ptr<triangle_hit_record> tri_rec = std::dynamic_pointer_cast<triangle_hit_record>(rec);

            if (tri_rec)
            {
				throw std::runtime_error("Error in hit record downcast to triangle");
                return color(0, 0, 0);
            }		

            auto c = barycentric_color_interpolation(tri_rec, t);

			if (c.has_value())
				return c.value();

            return 0.5 * (tri_rec->normal + WHITE);
            */
        }

        // Unknown hit
        return sky_blend(r);
    }

    color sky_blend(const ray& r) const
    {
        vec3 unit_direction = unit_vector(r.direction());

        auto a = 0.5 * (unit_direction.y() + 1.0);
        color start_color = WHITE;
        color end_color = SKY_BLUE;

        return lerp(a, start_color, end_color);
    }

    optional<color> barycentric_color_interpolation(const shared_ptr<triangle_hit_record>& rec, triangle* t) const
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
};

#endif
