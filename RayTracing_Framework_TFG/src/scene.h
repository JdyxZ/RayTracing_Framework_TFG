#ifndef SCENE_H
#define SCENE_H

class Scene : public hittable_list
{
public:

    // Scene properties
    string name;

    // Ray scattering settings
    int    bounce_max_depth = 10;       // Maximum number of ray bounces into scene
    double min_hit_distance = 0.001;    // Greatly solves shadow acne

    // Antialiasing and noise settings
    int    samples_per_pixel = 10;      // Count of random samples for each pixel

    // Scene background color
    bool sky_blend = true;
    color background = SKY_BLUE;

    // Hittable objects with PDF sampling distribution
    vector<shared_ptr<Hittable>> hittables_with_pdf;

    // Scene specs
    int bvh_depth = 0;
    int bvh_nodes = 0;
    int spheres = 0;
    int quads = 0;
    int triangles = 0;
    int emissives = 0;
    int primitives = 0;
    string _start;
    string _end;

    // Chronos
    Chrono full_pipeline;
    Chrono build_chrono;
    Chrono bvh_chrono;

    // Mesh vector for log support
    vector<shared_ptr<Mesh>> meshes;

    // Constructors
    Scene() 
    {
        build_chrono = Chrono();
        bvh_chrono = Chrono();
    }

    void start()
    {
        _start = get_current_timestamp("%c");
        full_pipeline.start();
    }

    void end()
    {
        _end = get_current_timestamp("%c");
        full_pipeline.end();
    }

    void add(shared_ptr<Hittable> object) override
    {
        hittable_list::add(object);

        if (object->has_pdf())
            hittables_with_pdf.push_back(object);

        switch (object->get_type())
        {
        case SPHERE:
            spheres++;
            primitives++;
            break;
        case TRIANGLE:
            triangles++;
            primitives++;
            break;
        case BOX:
        {
            auto box_ptr = std::dynamic_pointer_cast<Box>(object);

            bvh_chrono += box_ptr->bvh_chrono();

            quads += 6;
            primitives += 6;
            break;
        }
        case QUAD:
        {
            auto quad_ptr = std::dynamic_pointer_cast<Quad>(object);

            auto material = quad_ptr->get_material();
            if (material->get_type() == DIFFUSE_LIGHT) emissives++;

            quads++;
            primitives++;
            break;
        }
        case MESH: 
        {
            auto mesh_ptr = std::dynamic_pointer_cast<Mesh>(object);
            meshes.push_back(mesh_ptr);

            auto mesh_triangles = mesh_ptr->num_triangles();
            triangles += mesh_triangles;
            primitives += mesh_triangles;

            bvh_chrono += mesh_ptr->bvh_chrono();
            break;
        }
        case BVH_NODE:
        {
            auto bvh_node_ptr = std::dynamic_pointer_cast<bvh_node>(object);
            bvh_chrono += bvh_node_ptr->bvh_chrono();
            break;
        }
        }
    }
};

#endif
