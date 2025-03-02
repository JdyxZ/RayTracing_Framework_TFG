#include "core.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "triangle.h"
#include "camera.h"

int main() 
{
    // Scene 1
    hittable_list scene1;

    // Materials
    auto material_ground = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    auto material_center = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    auto material_left = make_shared<dielectric>(1.50);
    auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    // Spheres
    auto sphere1 = make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground);
    auto sphere2 = make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center);
    auto sphere3 = make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left);
	auto sphere4 = make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right);
    auto sphere5 = make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble);

	// Triangles (define them in counter-clockwise order)
    vertex A, B, C;
	A = vertex({ point3(0.2, -0.2, -0.5), nullopt, YELLOW });
    B = vertex({ point3(0.8, -0.2, -0.5), nullopt, CYAN });
	C = vertex({ point3(0.2, 0, -0.5), nullopt, MAGENTA });
	auto triangle1 = make_shared<triangle>(A, B, C, material_left);
    
    // Add primitives to scene
    scene1.add(sphere1);
	scene1.add(sphere2);
	// scene1.add(sphere3);
    // scene1.add(sphere4);
    // scene1.add(sphere5);
    // scene1.add(triangle1);

    // Scene 2
    hittable_list scene2;

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    scene2.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    scene2.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    scene2.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    scene2.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    scene2.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    scene2.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    scene2.add(make_shared<sphere>(point3(0, -1000, 0), 1000, material_ground));

    // scene2.add(triangle1);

    // Create and set camera 1 settings
    camera cam1;
    cam1.image_width = 800;
    cam1.bounce_max_depth = 50;
    cam1.vertical_fov = 90;
    cam1.defocus_angle = 0;
    cam1.samples_per_pixel = 50;

    // Create and set camera 2 settings
    camera cam2;
    cam2.aspect_ratio = 16.0 / 9.0;
    cam2.image_width = 1200;
    cam2.samples_per_pixel = 50;
    cam2.bounce_max_depth = 50;
    cam2.vertical_fov = 20;
    cam2.lookfrom = point3(13, 2, 3);
    cam2.lookat = point3(0, 0, 0);
    cam2.world_up = vec3(0, 1, 0);
    cam2.defocus_angle = 0.6;
    cam2.focus_distance = 10.0;

	// Initialize the camera
    cam2.initialize();

    // Render scene
    cam2.render(scene2);

    // Benchmark
    scene2.chrono.print_elapsed();
}
        