#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

shared_ptr<Mesh> load_obj(const string& obj_path)
{
	// Create tiny obj reader object
    tinyobj::ObjReaderConfig reader_config;
	tinyobj::ObjReader reader;

	// Define pahts
    reader_config.mtl_search_path = "./"; // Path to material files
    auto obj_path_fs = std::filesystem::path(obj_path);

    // Try to load obj file data into reader object
    if (!reader.ParseFromFile(obj_path, reader_config))
    {
        if (!reader.Error().empty()) 
            std::cerr << "TinyObjReader: " << reader.Error();

		return nullptr;
    }

	// Output loading warnings
    if (!reader.Warning().empty()) 
        std::cout << "TinyObjReader: " << reader.Warning();

    // Obj data
	auto& attrib = reader.GetAttrib(); // Vertex data (position, normal, color, texture coordinates)
    auto& shapes = reader.GetShapes(); // Submeshes inside the obj
	auto& materials = reader.GetMaterials(); // Materials included in the obj

    // Create mesh
    auto mesh = make_shared<Mesh>();

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) 
    {
        // Create new shape
        shared_ptr<Surface> surface;
        shared_ptr<hittable_list> triangles;
        shared_ptr<Material> material;

        if (!materials.empty())
        {
			// Get shape material id
            uint32_t material_id = shapes[s].mesh.material_ids[0];
			material_id = material_id == -1 ? 0 : material_id;

            // If there is no diffuse texture, create one
            if (materials[material_id].diffuse_texname.empty()) 
            {
                const color albedo = color(static_cast<double>(materials[material_id].diffuse[0]),
                    static_cast<double>(materials[material_id].diffuse[1]),
                    static_cast<double>(materials[material_id].diffuse[2]));

                material = make_shared<lambertian>(albedo);
			}
            // Else, load texture
            else
            {
                /*
                auto texture = make_shared<image_texture>(obj_path_fs.parent_path().string() + "/" + materials[material_id].diffuse_texname);
                material = make_shared<lambertian>(texture);
                */
                const color albedo = color(static_cast<double>(materials[material_id].diffuse[0]),
                    static_cast<double>(materials[material_id].diffuse[1]),
                    static_cast<double>(materials[material_id].diffuse[2]));

                material = make_shared<lambertian>(albedo);
            }
        }

        size_t index_offset = 0;

        // Loop over faces (primitives / polygon)
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) 
        {
            // Number of vertices per face
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Ignore non-triangle primitives
            if (fv != 3) 
                continue;

            // Create empty triangle
            shared_ptr<Triangle> triangle;
            vertex vertices[3];

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) 
            {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                // Vertex data pointer
                vertex& current_vertex = vertices[v];

                // Vertex position
                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
                current_vertex.position = point3(vx, vy, vz);

                // If normal_index is negative, there is no normal data
                if (idx.normal_index >= 0) 
                {
                    // Vertex normal
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    current_vertex.normal = point3(nx, ny, nz);
                }

                // If texcoord_index is negative, there is no texture coordinate data
                if (idx.texcoord_index >= 0) 
                {
					// Vertex texture coordinate
                    tinyobj::real_t u = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t v = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    current_vertex.uv = make_pair(u, v);
                }

                // Vertex color
                tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
                current_vertex.color = color(red, green, blue);           
            }

            // Create and add triangle
            triangle = make_shared<Triangle>(vertices[0], vertices[1], vertices[2], material);
            triangles->add(triangle);

            index_offset += fv;

            // per-face material
            // shapes[s].mesh.material_ids[f];
        }

        // Create and add surface
        surface = make_shared<Surface>(triangles, material);
        mesh->add(surface);
    }

    // Obj name
	string obj_name = obj_path_fs.stem().string();

    return mesh;
}

#endif
