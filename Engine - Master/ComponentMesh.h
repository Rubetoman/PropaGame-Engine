#ifndef __COMPOMENTMESH_H__
#define __COMPOMENTMESH_H__

#include "Component.h"
#include "Texture.h"
#include "GameObject.h"
#include "MathGeoLib.h"

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
	unsigned num_indices = 0u;
	std::vector<float3> vertices;

	AABB boundingBox;
};

#endif