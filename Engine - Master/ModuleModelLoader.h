#ifndef __MODULEMODELLOADER_h__
#define __MODULEMODELLOADER_h__

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
	ModuleModelLoader();
	~ModuleModelLoader();
	bool            Init();
	update_status   Update();
	bool            CleanUp();
	bool			LoadMesh(const char* path);
	void			LoadMaterial(const char* path);
	void			GenerateMeshData(const aiScene* scene);
	void			GenerateMaterialData(const aiScene* scene);
	void			ChangeMeshTexture(const char * path);
	void			DeleteMesh(int index);
	void			DeleteMaterial(int index);

private:
	unsigned vbo = 0; // Stores numers in case of error

public:
	struct mesh
	{
		std::string name;
		unsigned vbo = 0;
		unsigned ibo = 0;
		unsigned vao = 0;
		unsigned material = 0;
		unsigned num_vertices = 0;
		unsigned num_indices = 0;
		unsigned texture = 0;
		unsigned texWidth = 0;
		unsigned texHeight = 0;
		AABB boundingBox;
		math::float3 position = math::float3(0.0f,0.0f,0.0f);
		math::float3 scale = math::float3(0.0f, 0.0f, 0.0f);
	};

	struct material
	{
		unsigned texture0 = 0;
	};

	const aiScene* scene;
	std::vector<mesh*>     meshes;
	std::vector<material> materials;
	math::float4x4        transform = math::float4x4::identity;
};
#endif /* __MODULEMODELLOADER_h__ */
