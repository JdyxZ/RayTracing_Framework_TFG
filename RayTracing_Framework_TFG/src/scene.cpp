// Headers
#include "core.hpp"
#include "scene.hpp"
#include "utilities.hpp"
#include "chrono.hpp"
#include "hittable.hpp"
#include "box.hpp"
#include "quad.hpp"
#include "mesh.hpp"
#include "bvh.hpp"
#include "material.hpp"

Scene::Scene()
{
    full_pipeline = make_shared<Chrono>();
    build_chrono = make_shared<Chrono>();
    bvh_chrono = make_shared<Chrono>();
}

void Scene::start()
{
    _start = get_current_timestamp("%c");
    full_pipeline->start();
}

void Scene::end()
{
    _end = get_current_timestamp("%c");
    full_pipeline->end();
}

void Scene::add(shared_ptr<Hittable> object) 
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