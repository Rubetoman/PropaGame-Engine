#ifndef __MODULEMODELLOADER_H__
#define __MODULEMODELLOADER_H__

#include "Module.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "GameObject.h"

#include "MathGeoLib.h"
#include "GL/glew.h"
#include "MathGeoLib.h"
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <vector>
#include <iterator>

class ModuleModelLoader : public Module
{
public:

	ModuleModelLoader();
	~ModuleModelLoader();
	bool            Init();
	update_status   Update();
	bool            CleanUp();

	bool			LoadMesh(const char* path);
	//void			LoadMaterial(const char* path);
	void			GenerateNodeMeshData(const aiScene* scene, const aiNode* node, const aiMatrix4x4& parent_transform, GameObject* parent);
	void			ChangeMeshTexture(const char* path);

	// Shape Creation
	void CreateSphere(const char* name, const math::float3& position, const math::Quat& rotation, const math::float3& scale,
		unsigned slices, unsigned stacks, const math::float4& color);
	void CreateCylinder(const char* name, const math::float3& position, const math::Quat& rotation, float height,
		float radius, unsigned slices, unsigned stacks, const math::float4& color);

	const aiScene* scene = nullptr;
	//std::vector<mesh*>    meshes;
	//std::vector<material> materials;
};
#endif /* __MODULEMODELLOADER_H__ */
