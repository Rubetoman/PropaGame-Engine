#ifndef __COMPOMENTMESH_H__
#define __COMPOMENTMESH_H__

#include "Component.h"
#include "Geometry/AABB.h"

class Texture;
class GameObject;

struct par_shapes_mesh_s;

struct Mesh 
{
	unsigned vbo = 0u;
	unsigned ibo = 0u;
	unsigned vao = 0u;

	//unsigned		num_normals = 0u;
	float*		normals = nullptr;
	float*		colors = nullptr;
	float*		uvs = nullptr;

	unsigned	num_vertices = 0u;
	float*		vertices = nullptr;

	unsigned	num_indices = 0u;
	unsigned*	indices = nullptr;

	unsigned normalsOffset = 0u;
	unsigned texturesOffset = 0u;
	unsigned vertexSize = 0u;

	math::AABB boundingBox = math::AABB();
};

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* go);
	ComponentMesh(const ComponentMesh& comp);
	~ComponentMesh();

	virtual Component* Duplicate() override;
	void CleanUp() override;

	bool DrawOnInspector() override;

	void LoadMesh(const char* name);

	void ComputeMesh();
	void ComputeMesh(par_shapes_mesh_s* parMesh);

	void RenderMesh(const math::float4x4& view, const math::float4x4& proj);
	void DeleteMesh();
	void Delete() override;

	JSON_value* Save(JSON_value* component) const override;
	void Load(JSON_value* component) override;

public:

	Mesh mesh;
	std::string currentMesh;
};

#endif