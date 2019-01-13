#ifndef __COMPOMENTMESH_H__
#define __COMPOMENTMESH_H__

#include "Component.h"
#include "Geometry/AABB.h"

class Texture;
class GameObject;

struct par_shapes_mesh_s;

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

	void GenerateMesh(par_shapes_mesh_s* mesh);
	void RenderMesh(const math::float4x4& view, const math::float4x4& proj);
	void DeleteMesh();
	void Delete() override;

	JSON_value* Save(JSON_value* component) const override;
	void Load(JSON_value* component) override;

public:
	// Mesh variables
	unsigned vbo = 0u;
	unsigned ibo = 0u;
	unsigned vao = 0u;

	unsigned num_vertices = 0u;
	float* vertices = nullptr;

	unsigned num_indices = 0u;
	unsigned* indices = nullptr;

	unsigned num_normals = 0u;
	float* normals = nullptr;

	float* uvs = nullptr;
	float* colors = nullptr;

	unsigned normalsOffset = 0u;
	unsigned texturesOffset = 0u;
	unsigned vertexSize = 0u;

	math::AABB boundingBox = math::AABB();

private:
	std::string currentMesh;
};

#endif