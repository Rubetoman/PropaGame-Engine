#ifndef __MODULEMODELLOADER_h__
#define __MODULEMODELLOADER_h__

#include "Module.h"
#include "Application.h"
#include "GL/glew.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/mesh.h>

class ModuleModelLoader : public Module
{
public:
	ModuleModelLoader();
	~ModuleModelLoader();

	bool            Init();
	update_status   Update();
	bool            CleanUp();
	void			GenerateMeshData(aiMesh* mesh);
	void			GenerateMaterialData(aiMaterial* material);

	const aiScene* scene;
	int num_vertices = 0;
private:
	unsigned vbo = 0; // Stores numers in case of error
};

#endif /* __MODULEMODELLOADER_h__ */
