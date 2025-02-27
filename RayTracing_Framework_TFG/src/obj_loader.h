/*


#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "tiny_obj_loader.h"
#include "hittable_list.h"
#include "triangle.h"
#include <memory>
#include <string>

bool load_obj(const std::string& filename, hittable_list& scene, std::shared_ptr<material> mat) 
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    std::string aux1, aux2;
    bool aux2 = false;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(), aux2.c_str(), aux2))
    {
        std::cerr << warn << err << std::endl;
        return false;
    }

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            vec3 vertices[3];
            for (size_t i = 0; i < 3; ++i) {
                tinyobj::index_t idx = shape.mesh.indices[i];
                vertices[i] = vec3(
                    attrib.vertices[3 * idx.vertex_index + 0],
                    attrib.vertices[3 * idx.vertex_index + 1],
                    attrib.vertices[3 * idx.vertex_index + 2]
                );
            }
            scene.add(std::make_shared<triangle>(vertices[0], vertices[1], vertices[2], mat));
        }
    }

    return true;
}

#endif

*/
