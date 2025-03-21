#ifndef MESH_H
#define MESH_H

struct Mesh
{
public:
	Mesh() 
	{

	}
private:
	shared_ptr<bvh_node> triangles;
};

#endif

