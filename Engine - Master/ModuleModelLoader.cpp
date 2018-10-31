#include "ModuleModelLoader.h"

ModuleModelLoader::ModuleModelLoader()
{
}

ModuleModelLoader::~ModuleModelLoader()
{
}

bool ModuleModelLoader::Init()
{
	unsigned int postprocess_flags = 0;
	scene = aiImportFile("BakerHouse.fbx", postprocess_flags);
	if (scene == NULL)
	{
		aiGetErrorString();
	}

	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		GenerateMeshData(scene->mMeshes[i]);
	}
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		GenerateMaterialData(scene->mMaterials[i]);
	}

	return true;
}

update_status ModuleModelLoader::Update()
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
		0,                  // attribute 0
		3,                  // number of componentes (3 floats)
		GL_FLOAT,           // data type
		GL_FALSE,           // should be normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glDrawArrays(GL_POINTS, 0, num_vertices);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return UPDATE_CONTINUE;
}

bool ModuleModelLoader::CleanUp()
{
	// Delete loaded models

	return true;
}

//float vertex_buffer_data[]

void ModuleModelLoader::GenerateMeshData(aiMesh* mesh) 
{
	//float vertex_buffer_object[mesh->mNumVertices * 5] = mesh->mVertices;
	num_vertices += mesh->mNumVertices;
	glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 5, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->mNumVertices * 3, mesh->mVertices);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->mNumVertices * 2, mesh->mTextureCoords[0]);

	//glMapBufferRange(GL_ARRAY_BUFFER, 0, 2, 

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModuleModelLoader::GenerateMaterialData(aiMaterial* material)
{

}
