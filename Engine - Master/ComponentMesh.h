#ifndef __COMPOMENTMESH_H__
#define __COMPOMENTMESH_H__

#include "Component.h"
#include "ModuleModelLoader.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* go);
	~ComponentMesh();

	//void DrawOnInspector() override;

	void RenderMesh(unsigned program, const math::float4x4& view, const math::float4x4& proj);

	ModuleModelLoader::mesh* mesh;
};

#endif