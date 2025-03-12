// External definitions
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#define __STDC_LIB_EXT1__

// External libraries
#include "../external/stb_image.h"
#include "../external/stb_image_write.h" 
#include "../external/tiny_obj_loader.h"

// Framework headers
#include "core.h"
#include "benchmark.h"
#include "hittable.h"
#include "hittable_list.h"
#include "scene.h"
#include "bvh.h"
#include "image_writer.h"
#include "image_reader.h"
#include "perlin.h"
#include "texture.h"
#include "material.h"
#include "quad.h"
#include "box.h"
#include "sphere.h"
#include "triangle.h"
#include "camera.h"

void book1_final_scene_creation(Scene& scene, bool blur_motion = false)
{
    // Materials
    auto material_ground = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    auto material_center = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    auto material_left = make_shared<dielectric>(1.50);
    auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);
    auto material1 = make_shared<dielectric>(1.5);
    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);

    // Triangles (define them in counter-clockwise order)
    vertex A, B, C;
    A = vertex({ point3(0.2, -0.2, -0.5), nullopt, YELLOW });
    B = vertex({ point3(0.8, -0.2, -0.5), nullopt, CYAN });
    C = vertex({ point3(0.2, 0, -0.5), nullopt, MAGENTA });
    auto triangle1 = make_shared<Triangle>(A, B, C, material_left);

    // Random sphere creation loop
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<Material> sphere_material;

                // Diffuse
                if (choose_mat < 0.8)
                {
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);

                    if (blur_motion)
                    {
                        auto center2 = center + vec3(0, random_double(0, .5), 0);
                        scene.add(make_shared<Sphere>(center, center2, 0.2, sphere_material));
                    }
                    else
                    {
                        ;                       scene.add(make_shared<Sphere>(center, 0.2, sphere_material));
                    }
                }
                // Metal
                else if (choose_mat < 0.95)
                {
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    scene.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                // Glass
                else
                {
                    sphere_material = make_shared<dielectric>(1.5);
                    scene.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    // Spheres
    auto sphere1 = make_shared<Sphere>(point3(0, 1, 0), 1.0, material1);
    auto sphere2 = make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2);
    auto sphere3 = make_shared<Sphere>(point3(4, 1, 0), 1.0, material3);
    auto sphere4 = make_shared<Sphere>(point3(0, -1000, 0), 1000, material_ground);

    // Add primitives to the scene
    scene.add(sphere1);
    scene.add(sphere2);
    scene.add(sphere3);
    scene.add(sphere4);
    // scene1.add(triangle1);
}

void book1_final_scene(Scene& scene, Camera& camera, ImageWriter& image)
{
    // Image settings
    image.width = 1200;

    // Camera settings
    camera.vertical_fov = 20;
    camera.lookfrom = point3(13, 2, 3);
    camera.lookat = point3(0, 0, 0);
    camera.defocus_angle = 0.6;

    // Scene settings
    scene.bounce_max_depth = 50;
    scene.samples_per_pixel = 50;

    // Create scene
    book1_final_scene_creation(scene);
}

void bouncing_spheres(Scene& scene, Camera& camera, ImageWriter& image)
{
    // Image settings
    image.width = 1200;

    // Camera settings
    camera.vertical_fov = 20;
    camera.lookfrom = point3(13, 2, 3);
    camera.lookat = point3(0, 0, 0);
    camera.defocus_angle = 0.6;

    // Scene settings
    scene.bounce_max_depth = 50;
    scene.samples_per_pixel = 50;

    // Create scene
    book1_final_scene_creation(scene, true);
}

void checkered_spheres(Scene& scene, Camera& camera, ImageWriter& image)
{
    // Camera settings
    camera.vertical_fov = 20;
    camera.lookfrom = point3(13, 2, 3);
    camera.lookat = point3(0, 0, 0);

    // Scene settings
    scene.bounce_max_depth = 50;
    scene.samples_per_pixel = 100;

    // Materials
    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));

    // Spheres
    auto sphere1 = make_shared<Sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker));
    auto sphere2 = make_shared<Sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker));

    // Add primitives
    scene.add(sphere1);
    scene.add(sphere2);
}

void earth(Scene& scene, Camera& camera, ImageWriter& image)
{
    // Camera settings
    camera.vertical_fov = 20;
    camera.lookfrom = point3(0, 0, 12);
    camera.lookat = point3(0, 0, 0);

    // Scene settings
    scene.bounce_max_depth = 50;
    scene.samples_per_pixel = 100;

    // Textures
    auto earth_texture = make_shared<image_texture>("earthmap.jpg");

    // Materials
    auto earth_surface = make_shared<lambertian>(earth_texture);

    // Spheres
    auto globe = make_shared<Sphere>(point3(0, 0, 0), 2, earth_surface);

    // Add primitives
    scene.add(globe);
}

void perlin_spheres(Scene& scene, Camera& camera, ImageWriter& image)
{
    // Camera settings
    camera.vertical_fov = 20;
    camera.lookfrom = point3(13, 2, 3);
    camera.lookat = point3(0, 0, 0);

    // Scene settings
    scene.bounce_max_depth = 50;
    scene.samples_per_pixel = 100;

    // Textures
    auto perlin_texture = make_shared<noise_texture>(4, 7);

    // Materials
    auto perlin_material = make_shared<lambertian>(perlin_texture);

    // Spheres
    auto sphere1 = make_shared<Sphere>(point3(0, -1000, 0), 1000, perlin_material);
    auto sphere2 = make_shared<Sphere>(point3(0, 2, 0), 2, perlin_material);

    // Add primitives
    scene.add(sphere1);
    scene.add(sphere2);
}

void quads(Scene& scene, Camera& camera, ImageWriter& image)
{
    // Image settings
    image.aspect_ratio = 1.0;

    // Camera settings
    camera.vertical_fov = 80;
    camera.lookfrom = point3(0, 0, 9);
    camera.lookat = point3(0, 0, 0);

    // Scene settings
    scene.bounce_max_depth = 50;
    scene.samples_per_pixel = 100;

    // Materials
    auto left_red = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    auto quad1 = make_shared<Quad>(point3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), left_red);
    auto quad2 = make_shared<Quad>(point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green);
    auto quad3 = make_shared<Quad>(point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue);
    auto quad4 = make_shared<Quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange);
    auto quad5 = make_shared<Quad>(point3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4), lower_teal);

    // Add primitives
    scene.add(quad1);
    scene.add(quad2);
    scene.add(quad3);
    scene.add(quad4);
    scene.add(quad5);
}

void simple_light(Scene& scene, Camera& camera, ImageWriter& image) 
{
    // Image settings
    image.aspect_ratio = 1.0;

    // Camera settings
    camera.vertical_fov = 20;
    camera.lookfrom = point3(26, 3, 6);
    camera.lookat = point3(0, 2, 0);

    // Scene settings
    scene.sky_blend = false;
    scene.background = BLACK;
    scene.bounce_max_depth = 50;
    scene.samples_per_pixel = 100;

    // Textures
    auto perlin_texture = make_shared<noise_texture>(4, 7);

    // Materials
    auto perlin_material = make_shared<lambertian>(perlin_texture);
    auto diffuse_light_material = make_shared<diffuse_light>(color(4, 4, 4));

    // Spheres
    auto sphere1 = make_shared<Sphere>(point3(0, -1000, 0), 1000, perlin_material);
    auto sphere2 = make_shared<Sphere>(point3(0, 2, 0), 2, perlin_material);
    auto sphere3 = make_shared<Sphere>(point3(0, 7, 0), 2, diffuse_light_material);

    // Quads
    auto quad1 = make_shared<Quad>(point3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0), diffuse_light_material);

    // Add primitives
    scene.add(sphere1);
    scene.add(sphere2);
    scene.add(sphere3);
    scene.add(quad1);
}

void cornell_box(Scene& scene, Camera& camera, ImageWriter& image)
{
    // Image settings
    image.aspect_ratio = 1.0;
    image.width = 600;

    // Camera settings
    camera.vertical_fov = 40;
    camera.lookfrom = point3(278, 278, -800);
    camera.lookat = point3(278, 278, 0);

    // Scene settings
    scene.sky_blend = false;
    scene.background = BLACK;
    scene.bounce_max_depth = 50;
    scene.samples_per_pixel = 200;

    // Materials
    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    // Quads
    auto quad1 = make_shared<Quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green);
    auto quad2 = make_shared<Quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red);
    auto quad3 = make_shared<Quad>(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), light);
    auto quad4 = make_shared<Quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white);
    auto quad5 = make_shared<Quad>(point3(555, 555, 555), vec3(-555, 0, 0), vec3(0, 0, -555), white);
    auto quad6 = make_shared<Quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white);
    
    // Boxes
	auto box1 = make_shared<Box>(point3(130, 0, 65), point3(295, 165, 230), white);
	auto box2 = make_shared<Box>(point3(265, 0, 295), point3(430, 330, 460), white);

    // Add primitives
    scene.add(quad1);
    scene.add(quad2);
    scene.add(quad3);
    scene.add(quad4);
    scene.add(quad5);
    scene.add(quad6);
    scene.add(box1);
    scene.add(box2);
}

int main()
{
    // Create render objects
    Scene scene;
    Camera camera;
    ImageWriter image;

    // Choose rendering scene
    switch (7)
    {
    case 0:
        book1_final_scene(scene, camera, image);
        break;
    case 1:
        bouncing_spheres(scene, camera, image);
        break;
    case 2:
        checkered_spheres(scene, camera, image);
        break;
    case 3:
        earth(scene, camera, image);
        break;
    case 4:  
        perlin_spheres(scene, camera, image);     
        break;
    case 5:  
        quads(scene, camera, image);
        break;
    case 6:  
        simple_light(scene, camera, image);
        break;
    case 7:  
        cornell_box(scene, camera, image);
        break;
    }

    // Boost scene render with BVH
    auto BVH_tree = make_shared<bvh_node>(scene);
    scene.clear();
    scene.add(BVH_tree);

    // Intialize image
    image.initialize();

    // Initialize the camera
    camera.initialize(image);

    // Render scene
    camera.render(scene, image);

    // Benchmark
    scene.chrono.print_elapsed();
}
        