#include "ModuleModelLoader.h"

ModuleModelLoader::ModuleModelLoader()
{
}
ModuleModelLoader::~ModuleModelLoader()
{
}
bool ModuleModelLoader::Init()
{
	return LoadMesh("Assets/Models/BakerHouse.fbx");
}
update_status ModuleModelLoader::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleModelLoader::CleanUp()
{
	// Delete loaded models
	for (unsigned i = 0; i < meshes.size(); ++i)
	{
		DeleteMesh(i);
	}

	for (unsigned i = 0; i < materials.size(); ++i)
	{
		DeleteMaterial(i);
	}
	return true;
}

bool ModuleModelLoader::LoadMesh(const char* path)
{
	assert(path != nullptr);
	unsigned int postprocess_flags = aiProcessPreset_TargetRealtime_MaxQuality;
	scene = aiImportFile(path, postprocess_flags);
	if (scene == NULL)
	{
		aiGetErrorString();
		return false;
	}

	GenerateMeshData(scene);
	GenerateMaterialData(scene);
	aiReleaseImport(scene);
	return true;
}

void ModuleModelLoader::LoadMaterial(const char* path)
{
	assert(path != nullptr);
	const aiMaterial* src_material = nullptr;
	material gen_material;

	aiString file;
	aiTextureMapping mapping;
	unsigned uvindex = 0;

	if (src_material->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, &uvindex) == AI_SUCCESS)
	{
		gen_material.texture0 = App->textures->loadTexture(file.data);
	}

	materials.push_back(gen_material);
}

void ModuleModelLoader::GenerateMeshData(const aiScene* scene)
{
	assert(scene != nullptr);
	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		const aiMesh* src_mesh = scene->mMeshes[i];

		// Generate mesh object
		mesh gen_mesh;
		// vertex array objects (VAO)
		//glGenVertexArrays(1, &gen_mesh.vao);
		glGenBuffers(1, &gen_mesh.vbo);
		//glBindVertexArray(gen_mesh.vao);
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
		
		//glBindVertexArray(0);

		gen_mesh.material = src_mesh->mMaterialIndex;
		gen_mesh.num_vertices = src_mesh->mNumVertices;
		gen_mesh.num_indices = src_mesh->mNumFaces * 3;

		meshes.push_back(gen_mesh);
	}
}

void ModuleModelLoader::GenerateMaterialData(const aiScene* scene)
{
	assert(scene != nullptr);
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		const aiMaterial* src_material = scene->mMaterials[i];

		material gen_material;
		
		aiString file;
		aiTextureMapping mapping;
		unsigned uvindex = 0;

		if (src_material->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, &uvindex) == AI_SUCCESS)
		{
			gen_material.texture0 = App->textures->loadTexture(file.data);
			if (!gen_material.texture0)
			{
				LOG("Texture couldn't be found on Game dir, looking on Assets/Models/");
				char * path = (char *)malloc(1 + strlen("Assets/Models/") + strlen(file.data));
				strcpy(path, "Assets/Models/");
				strcat(path, file.data);
				gen_material.texture0 = App->textures->loadTexture(path);
				free(path);
			}
			if (!gen_material.texture0)
			{
				LOG("Texture couldn't be found on Game dir, looking on Assets/Models/textures/");
				char * path = (char *)malloc(1 + strlen("Assets/Models/textures/") + strlen(file.data));
				strcpy(path, "Assets/Models/textures/");
				strcat(path, file.data);
				gen_material.texture0 = App->textures->loadTexture(path);
				free(path);
			}
		}
		materials.push_back(gen_material);
	}
}

void ModuleModelLoader::ChangeMeshTexture(const char * path)
{
	assert(path != nullptr);
	unsigned width, height;
	GLuint tex_id = App->textures->loadTexture(path);

	for (std::vector<material>::iterator it_m = materials.begin(); it_m != materials.end(); it_m++)
	{
		//glDeleteTextures(1, (GLuint*)(*it_m)->texture);
		(it_m)->texture0 = tex_id;
		//(it_m)->texWidth = width;
		//(it_m)->texHeight = height;
	}
}

void ModuleModelLoader::DeleteMesh(int index)
{
	if (meshes[index].vbo != 0)
	{
		glDeleteBuffers(1, &meshes[index].vbo);
	}

	if (meshes[index].ibo != 0)
	{
		glDeleteBuffers(1, &meshes[index].ibo);
	}
	
	if (meshes[index].vao != 0)
	{
		glDeleteBuffers(1, &meshes[index].vao);
	}
}

void ModuleModelLoader::DeleteMaterial(int index)
{
	if (materials[index].texture0 != 0)
	{
		App->textures->unloadTexture(materials[index].texture0);
	}
}
