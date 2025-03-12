#ifndef SCENE_H
#define SCENE_H

class Scene : public hittable_list
{
public:

    // Ray scattering settings
    int    bounce_max_depth = 10;       // Maximum number of ray bounces into scene
    double min_hit_distance = 0.001;    // Greatly solves shadow acne

    // Antialiasing and noise settings
    int    samples_per_pixel = 10;      // Count of random samples for each pixel

    // Scene background color
    bool sky_blend = true;
    color background = SKY_BLUE;

    // Rendering benchmark
    Benchmark chrono;

    Scene() {}
    Scene(shared_ptr<Hittable> object) { add(object); }
	Scene(shared_ptr<hittable_list> list) { add(list); }
};

#endif
