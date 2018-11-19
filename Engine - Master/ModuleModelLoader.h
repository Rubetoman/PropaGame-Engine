#ifndef __MODULEMODELLOADER_H__
#define __MODULEMODELLOADER_H__

#include "Module.h"
#include "Application.h"
#include "ModuleTextures.h"
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

	struct mesh
	{
		std::string name;
		unsigned vbo = 0u;
		unsigned ibo = 0u;
		unsigned vao = 0u;
		unsigned num_vertices = 0u;
		unsigned num_indices = 0u;
		Texture* texture;

		AABB boundingBox;
		math::float3 position = math::float3(0.0f, 0.0f, 0.0f);
		math::float3 scale = math::float3(0.0f, 0.0f, 0.0f);
	};


	ModuleModelLoader();
	~ModuleModelLoader();
	bool            Init();
	update_status   Update();
	bool            CleanUp();

	bool			LoadMesh(const char* path);
	//void			LoadMaterial(const char* path);
	void			GenerateMeshData(const aiScene* scene);
	void			ChangeMeshTexture(const char* path);
	void			DeleteMesh(const int index);
	//void			DeleteMaterial(const int index);

	const aiScene* scene = nullptr;
	std::vector<mesh*>    meshes;
	//std::vector<material> materials;
	math::float4x4        transform = math::float4x4::identity;
};
#endif /* __MODULEMODELLOADER_H__ */
