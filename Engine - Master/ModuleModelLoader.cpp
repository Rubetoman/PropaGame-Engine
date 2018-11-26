#include "ModuleModelLoader.h"
#include "ModuleCamera.h"
#include "ModuleScene.h"

#include "ComponentMesh.h"

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
	/*for (unsigned i = 0u; i < meshes.size(); ++i)
	{
		DeleteMesh(i);
	}
	meshes.clear();*/

	/*for (unsigned i = 0u; i < materials.size(); ++i)
	{
		DeleteMaterial(i);
	}*/
	return true;
}

bool ModuleModelLoader::LoadMesh(const char* path)
{
	assert(path != nullptr);
	unsigned int postprocess_flags = aiProcessPreset_TargetRealtime_MaxQuality;
	scene = aiImportFile(path, postprocess_flags);
	if (scene == nullptr)
	{
		LOG("Error, the file couldn't be loaded: %s \n", aiGetErrorString());
		return false;
	}
	App->camera->BBtoLook = new AABB({ .0f, .0f, .0f }, { .0f, .0f, .0f });

	GenerateMeshData(scene);
	aiReleaseImport(scene);
	return true;
}

/*void ModuleModelLoader::LoadMaterial(const char* path)
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
}*/

void ModuleModelLoader::GenerateMeshData(const aiScene* scene)
{
	assert(scene != nullptr);
	// Warning: Create parent GO for root
	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		const aiMesh* src_mesh = scene->mMeshes[i];

		// Generate Game Object with mesh component
		aiNode* root = scene->mRootNode;
		GameObject* go = new GameObject(src_mesh->mName.C_Str(), root->mTransformation);
		ComponentMesh* mesh = (ComponentMesh*)go->CreateComponent(component_type::Mesh);
		App->scene->game_objects.push_back(go);

		// vertex array objects (VAO)
		glGenVertexArrays(1, &mesh->mesh->vao);
		glBindVertexArray(mesh->mesh->vao);

		glGenBuffers(1, &mesh->mesh->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->mesh->vbo);

		// Divide Buffer for position and UVs
		glBufferData(GL_ARRAY_BUFFER, src_mesh->mNumVertices * (sizeof(float) * 3 + sizeof(float) * 2), nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * src_mesh->mNumVertices, src_mesh->mVertices);

		// Texture coords (UVs)
		// MapBufferRange because we only want UV data from UVW
		math::float2* texCoords = (math::float2*)glMapBufferRange(GL_ARRAY_BUFFER, sizeof(float) * 3 * src_mesh->mNumVertices,
			sizeof(float) * 2 * src_mesh->mNumVertices, GL_MAP_WRITE_BIT);

		for (unsigned j = 0; j < src_mesh->mNumVertices; ++j)
		{
			texCoords[j] = math::float2(src_mesh->mTextureCoords[0][j].x, src_mesh->mTextureCoords[0][j].y);
		}

		glUnmapBuffer(GL_ARRAY_BUFFER);

		// Indices (faces)
		glGenBuffers(1, &mesh->mesh->ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mesh->ibo);

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

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,                  // attribute 0
			3,                  // number of componentes (3 floats)
			GL_FLOAT,           // data type
			GL_FALSE,           // should be normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,                  // attribute 0
			2,                  // number of componentes (3 floats)
			GL_FLOAT,           // data type
			GL_FALSE,           // should be normalized?
			0,                  // stride
			(void*)(sizeof(float) * 3 * src_mesh->mNumVertices)       // array buffer offset
		);

		// Disable VAO
		glBindVertexArray(0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Disable VBO and EBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//gen_mesh->material = src_mesh->mMaterialIndex;
		mesh->mesh->num_vertices = src_mesh->mNumVertices;
		mesh->mesh->num_indices = src_mesh->mNumFaces * 3;

		mesh->mesh->boundingBox.SetNegativeInfinity();
		mesh->mesh->boundingBox.Enclose((math::float3*)src_mesh->mVertices, mesh->mesh->num_vertices);
		App->camera->BBtoLook->Enclose(mesh->mesh->boundingBox);

		// Load material
		const aiMaterial* src_material = scene->mMaterials[src_mesh->mMaterialIndex];

		aiString file;
		aiTextureMapping mapping;
		unsigned uvindex = 0;

		if (src_material->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, &uvindex) == AI_SUCCESS)
		{
			mesh->mesh->texture = App->textures->loadTexture(file.data);
		}
		else
		{
			LOG("Couldn't read the texture from .fbx file");
		}
	}
	App->camera->FitCamera(*App->camera->BBtoLook);
}

void ModuleModelLoader::ChangeMeshTexture(const char * path)
{
	assert(path != nullptr);
	unsigned width, height;
	Texture* tex = App->textures->loadTexture(path);

	//App->textures->unloadTexture((*meshes.begin())->texture);

	/*for (std::vector<mesh*>::iterator it_m = meshes.begin(); it_m != meshes.end(); it_m++)
	{
		(*it_m)->texture = tex;
	}*/
}

/*void ModuleModelLoader::DeleteMesh(const int index)
{
	if (meshes[index]->vbo != 0)
	{
		glDeleteBuffers(1, &meshes[index]->vbo);
	}

	if (meshes[index]->ibo != 0)
	{
		glDeleteBuffers(1, &meshes[index]->ibo);
	}
	
	if (meshes[index]->vao != 0)
	{
		glDeleteBuffers(1, &meshes[index]->vao);
	}
}*/

/*void ModuleModelLoader::DeleteMaterial(const int index)
{
	if (materials[index].texture0 != 0)
	{
		App->textures->unloadTexture(materials[index].texture0);
	}
}*/
