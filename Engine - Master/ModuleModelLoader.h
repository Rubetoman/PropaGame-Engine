#ifndef __MODULEMODELLOADER_h__
#define __MODULEMODELLOADER_h__

#include "Module.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "MathGeoLib.h"
#include "GL/glew.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <vector>

class ModuleModelLoader : public Module
{
public:
	ModuleModelLoader();
	~ModuleModelLoader();
	bool            Init();
	update_status   Update();
	bool            CleanUp();
	void			GenerateMeshData(const aiScene* scene);
	void			GenerateMaterialData(const aiScene* scene);
	const aiScene* scene;

private:
	unsigned vbo = 0; // Stores numers in case of error

public:
	struct mesh
	{
		unsigned vbo = 0;
		unsigned ibo = 0;
		unsigned vao = 0;
		unsigned material = 0;
		unsigned num_vertices = 0;
		unsigned num_indices = 0;
	};

	struct material
	{
		unsigned texture0 = 0;
	};

	std::vector<mesh>     meshes;
	std::vector<material> materials;
	math::float4x4        transform = math::float4x4::identity;
};
#endif /* __MODULEMODELLOADER_h__ */
