#include "ModuleModelLoader.h"
ModuleModelLoader::ModuleModelLoader()
{
}
ModuleModelLoader::~ModuleModelLoader()
{
}
bool ModuleModelLoader::Init()
{
	unsigned int postprocess_flags = aiProcessPreset_TargetRealtime_MaxQuality;
	scene = aiImportFile("BakerHouse.fbx", postprocess_flags);
	if (scene == NULL)
	{
		aiGetErrorString();
		return false;
	}

	GenerateMeshData(scene);
	//GenerateMaterialData(scene);

	return true;
}
update_status ModuleModelLoader::Update()
{
	/*glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, meshes[0].vbo);
	glVertexAttribPointer(
		0,                  // attribute 0
		3,                  // number of componentes (3 floats)
		GL_FLOAT,           // data type
		GL_FALSE,           // should be normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	
	glDrawArrays(GL_POINTS, 0, meshes[0].num_vertices);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/
	
	glActiveTexture(GL_TEXTURE0);

	// draw mesh
	glBindVertexArray(meshes[0].vao);
	glDrawElements(GL_TRIANGLES, meshes[0].num_indices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	return UPDATE_CONTINUE;
}

bool ModuleModelLoader::CleanUp()
{
	// Delete loaded models
	for (unsigned i = 0; i < meshes.size(); ++i)
	{
		if (meshes[i].vbo != 0)
		{
			glDeleteBuffers(1, &meshes[i].vbo);
		}

		if (meshes[i].ibo != 0)
		{
			glDeleteBuffers(1, &meshes[i].ibo);
		}
	}

	for (unsigned i = 0; i < materials.size(); ++i)
	{
		if (materials[i].texture0 != 0)
		{
			App->textures->unloadImage(materials[i].texture0);
		}
	}
	return true;
}
//float vertex_buffer_data[]
void ModuleModelLoader::GenerateMeshData(const aiScene* scene)
{
	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		const aiMesh* src_mesh = scene->mMeshes[i];

		// Generate mesh object
		mesh gen_mesh;
		glGenVertexArrays(1, &gen_mesh.vao);
		glGenBuffers(1, &gen_mesh.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, gen_mesh.vbo);

		// Divide Buffer for position and UVs
		glBufferData(GL_ARRAY_BUFFER, src_mesh->mNumVertices * (sizeof(float) * 3 + sizeof(float) * 2), nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * src_mesh->mNumVertices, src_mesh->mVertices);

		// Texture coords (UVs)
		// MapBufferRange because we only want UV data from UVW
		float2* texCoords = (float2*)glMapBufferRange(GL_ARRAY_BUFFER, sizeof(float) * 3 * src_mesh->mNumVertices,
			sizeof(float) * 2 * src_mesh->mNumVertices, GL_MAP_WRITE_BIT);

		for (unsigned j = 0; j < src_mesh->mNumVertices; ++j)
		{
			texCoords[j] = float2(src_mesh->mTextureCoords[0][j].x, src_mesh->mTextureCoords[0][j].y);
		}

		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Indices (faces)

		glGenBuffers(1, &gen_mesh.ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gen_mesh.ibo);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, src_mesh->mNumFaces * (sizeof(unsigned) * 3), nullptr, GL_STATIC_DRAW);

		// Texture coords (UVs)
		// MapBufferRange because we only want UV data from UVW
		unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0,
			sizeof(unsigned)*src_mesh->mNumFaces * 3, GL_MAP_WRITE_BIT);

		for (unsigned j = 0; j < src_mesh->mNumFaces; ++j)
		{
			assert(src_mesh->mFaces[j].mNumIndices == 3);

			*(indices++) = src_mesh->mFaces[j].mIndices[0];
			*(indices++) = src_mesh->mFaces[j].mIndices[1];
			*(indices++) = src_mesh->mFaces[j].mIndices[2];
		}

		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 2 * src_mesh->mNumVertices, src_mesh->mTextureCoords[0]);
		//glMapBufferRange(GL_ARRAY_BUFFER, 0, 2, 
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		gen_mesh.material = src_mesh->mMaterialIndex;
		gen_mesh.num_vertices = src_mesh->mNumVertices;
		gen_mesh.num_indices = src_mesh->mNumFaces * 3;

		meshes.push_back(gen_mesh);
	}
}

void ModuleModelLoader::GenerateMaterialData(const aiScene* scene)
{
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		const aiMaterial* src_material = scene->mMaterials[i];

		material gen_material;
		
		aiString file;
		aiTextureMapping mapping;
		unsigned uvindex = 0;

		if (src_material->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, &uvindex) == AI_SUCCESS)
		{
			image texture;
			texture.path = file.data;
			gen_material.texture0 = App->textures->loadImage(texture);
		}

		materials.push_back(gen_material);
	}
}
