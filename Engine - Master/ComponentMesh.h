#ifndef __COMPOMENTMESH_H__
#define __COMPOMENTMESH_H__

#include "Component.h"
#include "Texture.h"
#include "GameObject.h"
#include "MathGeoLib.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* go);
	ComponentMesh(const ComponentMesh& comp);
	~ComponentMesh();

	virtual Component* Duplicate() override;
	void CleanUp() override;

	void DrawOnInspector() override;

	void RenderMesh(unsigned program, const Texture* texture, const math::float4x4& view, const math::float4x4& proj);
	void DeleteMesh();
	void Delete() override;

	// Mesh variables
	unsigned vbo = 0u;
	unsigned ibo = 0u;
	unsigned vao = 0u;
	unsigned num_vertices = 0u;
	unsigned num_indices = 0u;

	AABB boundingBox;
};

#endif