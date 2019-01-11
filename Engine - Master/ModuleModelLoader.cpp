#include "Application.h"

#include "ModuleTextures.h"
#include "ModuleModelLoader.h"
#include "ModuleCamera.h"
#include "ModuleScene.h"
#include "ModuleResources.h"

#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#include "GameObject.h"
#include "MeshImporter.h"

#include "GL/glew.h"
#include "MathGeoLib.h"
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/mesh.h>

#define PAR_SHAPES_IMPLEMENTATION
#include "par_shapes.h"
#pragma warning(pop)

ModuleModelLoader::ModuleModelLoader()
{
}

ModuleModelLoader::~ModuleModelLoader()
{
}

bool ModuleModelLoader::Init()
{
	//CreateSphere("sphere", math::float3(0.0f, 0.0f, 0.0f), Quat::identity, math::float3(1.0f, 1.0f, 1.0f), 20, 20, float4(0.f, 0.0f, 0.5f, 1.0f));
	//CreateCylinder("cylinder0", math::float3(0.0f, 0.0f, 0.0f), Quat::identity, 2.0f, 1.0f, 30, 30, float4(0.f, 0.5f, 0.5f, 1.0f));

	//return LoadMesh("Assets/Models/BakerHouse.fbx");

	MeshImporter meshImporter;
	std::string outputfile = "BakerHouse.proMesh";
	meshImporter.Import("BakerHouse.fbx", "Assets/Models/BakerHouse.fbx", outputfile);
	char* bakerhouse = nullptr;
	App->file->LoadFileData(outputfile, bakerhouse);
	return LoadMesh(bakerhouse);
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

bool ModuleModelLoader::LoadMesh(char* data)
{
	assert(data != nullptr);
	if (data == nullptr)
	{
		LOG("No content to load!!");
		return false;
	}
	char* cursor = data;
	GenerateNodeMeshData(data, float4x4::identity, App->scene->root);
	//delete[] data;

	return true;
}

GameObject* ModuleModelLoader::GenerateNodeMeshData(char* &cursor, const math::float4x4& parent_transform, GameObject* parent)
{
	assert(cursor != nullptr);

	int id = *(int*)cursor;
	cursor += sizeof(int);

	int parent_id = *(int*)cursor;
	cursor += sizeof(int);

	float4x4 mytransform;
	mytransform.Set((float*)cursor);
	cursor += sizeof(float) * 16;

	float4x4 transform = parent_transform * mytransform;

	std::string name(cursor);
	cursor += (name.length()+1) * sizeof(char);
	GameObject* go = App->scene->CreateGameObject(name.c_str(), transform, parent); //TODO: refactor filepath variable

	unsigned int numMeshes = *(int*)cursor;
	cursor += sizeof(int);

	for (unsigned int i = 0; i < numMeshes; i++)
	{
		// Add Mesh Component
		ComponentMesh* mesh = (ComponentMesh*)go->CreateComponent(component_type::Mesh);
		mesh->GenerateMesh(cursor); //SetMesh moves the cursor at the end of the mesh

		std::string materialFile(cursor);
		cursor += sizeof(char)*(materialFile.length() + 1);
		ComponentMaterial* material = (ComponentMaterial*)go->CreateComponent(component_type::Material);
		material->GenerateMaterial(cursor); //TODO: Generate material with all textures
	}

	unsigned int numChildren = *(int*)cursor;
	cursor += sizeof(int);

	for (unsigned int i = 0; i < numChildren; i++)
	{
		GameObject* child = GenerateNodeMeshData(cursor, transform, go);

	}

	return go;
}

/*void ModuleModelLoader::GenerateNodeMeshData(const aiScene* scene, const aiNode* node, const aiMatrix4x4& parent_transform, GameObject* parent)
{
	assert(scene != nullptr); assert(node != nullptr);

	for (unsigned i = 0; i < node->mNumMeshes; ++i)
	{
		aiMatrix4x4 transform = parent_transform * node->mTransformation;
		GameObject* go = App->scene->CreateGameObject(node->mName.C_Str(), (math::float4x4&)transform, parent);

		// Avoid creating GO without name or too long
		if (go->name.size() < 1 || node->mName.length > GO_NAME_SIZE)
			go->name = GO_DEFAULT_NAME;

		// Add Mesh Component
		ComponentMesh* mesh = (ComponentMesh*)go->CreateComponent(component_type::Mesh);

		const aiMesh* src_mesh = scene->mMeshes[node->mMeshes[i]];

		// vertex array objects (VAO)
		glGenVertexArrays(1, &mesh->vao);
		glBindVertexArray(mesh->vao);

		glGenBuffers(1, &mesh->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);

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
		glGenBuffers(1, &mesh->ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);

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
		mesh->num_vertices = src_mesh->mNumVertices;
		mesh->num_indices = src_mesh->mNumFaces * 3;

		//Copying Vertices array
		mesh->vertices = new float[mesh->num_vertices * 3]; //It is checked below that at least has 1 face, so at least 3 vertices
		memcpy(mesh->vertices, src_mesh->mVertices, sizeof(float)*mesh->num_vertices * 3);

		//Copying Face Normals
		if (src_mesh->HasNormals())
		{
			mesh->num_normals = src_mesh->mNumVertices;
			mesh->normals = new float[mesh->num_normals * 3];
			memcpy(mesh->normals, src_mesh->mNormals, sizeof(float)*mesh->num_normals * 3);
		}

		//Copying indices
		mesh->num_indices = src_mesh->mNumFaces * 3;
		mesh->indices = new unsigned[mesh->num_indices]; // assume each face is a triangle

		for (int j = 0; j < src_mesh->mNumFaces; ++j)
		{
			if (src_mesh->mFaces[j].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
				LOG("WARNING, face normals couldn't be loaded");
				mesh = nullptr;
				break;
			}
			else
			{
				memcpy(&mesh->indices[j * 3], src_mesh->mFaces[j].mIndices, 3 * sizeof(unsigned));
			}
		}

		mesh->boundingBox.SetNegativeInfinity();
		mesh->boundingBox.Enclose((math::float3*)src_mesh->mVertices, mesh->num_vertices);
		App->resources->meshes.push_back(go->mesh);

		const aiMaterial* src_material = scene->mMaterials[src_mesh->mMaterialIndex];

		if (src_material != nullptr)
		{
			// Load material
			ComponentMaterial* material = (ComponentMaterial*)go->CreateComponent(component_type::Material);

			aiString file;
			aiTextureMapping mapping;
			unsigned uvindex = 0;

			if (src_material->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, &uvindex) == AI_SUCCESS)
			{
				go->material->diffuse_map = App->textures->loadTexture(file.data, false);
				if (go->material != nullptr)
					go->material->diffuse_color = math::float4::one;
			}
			else
			{
				LOG("Couldn't read the texture from .fbx file");
			}
		}

		if (go->mesh != nullptr)
		{
			App->camera->BBtoLook->Enclose(go->mesh->boundingBox);
			App->camera->FitCamera(*App->camera->BBtoLook);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			GenerateNodeMeshData(scene, node->mChildren[i], transform, go);
		}

	}

	if(node->mNumMeshes == 0)
	{
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			GenerateNodeMeshData(scene, node->mChildren[i], parent_transform, parent);
		}
	}
}*/

GameObject* ModuleModelLoader::CreateSphere(const char* name, const math::float3& position, const math::Quat& rotation, const math::float3& scale,
	unsigned slices, unsigned stacks, const math::float4& color)
{
	par_shapes_mesh* mesh = par_shapes_create_parametric_sphere(int(slices), int(stacks));

	if (mesh == nullptr)
	{
		LOG("Error: Sphere couldn't be created. Par_shapes returned nullptr.");
		return nullptr;
	}

	par_shapes_scale(mesh, scale.x, scale.y, scale.z);
	
	GameObject* sphere = App->scene->CreateGameObject(name, float4x4::FromTRS(position, rotation, scale));
	
	ComponentMesh* sphere_mesh = (ComponentMesh*)sphere->CreateComponent(component_type::Mesh);
	sphere_mesh->GenerateMesh(mesh);

	par_shapes_free_mesh(mesh);

	ComponentMaterial* sphere_material = (ComponentMaterial*)sphere->CreateComponent(component_type::Material);
	sphere_material->diffuse_map = App->textures->loadTexture("Checkers_Texture.jpg", false);

	return sphere;
}

GameObject* ModuleModelLoader::CreateCylinder(const char* name, const math::float3& position, const math::Quat& rotation, float height,
	float radius, unsigned slices, unsigned stacks, const math::float4& color)
{
	par_shapes_mesh* mesh = par_shapes_create_parametric_sphere(int(slices), int(stacks));
	par_shapes_rotate(mesh, -float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(mesh, 0.0f, -0.5f, 0.0f);

	par_shapes_mesh* top = par_shapes_create_disk(radius, int(slices), (const float*)&math::float3::zero, (const float*)&math::float3::unitZ);
	par_shapes_rotate(top, -float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(top, 0.0f, height*0.5f, 0.0f);

	par_shapes_mesh* bottom = par_shapes_create_disk(radius, int(slices), (const float*)&math::float3::zero, (const float*)&math::float3::unitZ);
	par_shapes_rotate(bottom, float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(bottom, 0.0f, height*-0.5f, 0.0f);

	if (mesh == nullptr)
	{
		LOG("Error: Cylinder couldn't be created. Par_shapes returned nullptr.");
		return nullptr;
	}

	par_shapes_scale(mesh, radius, height, radius);
	par_shapes_merge_and_free(mesh, top);
	par_shapes_merge_and_free(mesh, bottom);

	GameObject* cylinder = App->scene->CreateGameObject(name, float4x4::FromTRS(position, rotation, math::float3::one));

	ComponentMesh* cylinder_mesh = (ComponentMesh*)cylinder->CreateComponent(component_type::Mesh);
	cylinder_mesh->GenerateMesh(mesh);

	par_shapes_free_mesh(mesh);

	ComponentMaterial* cylinder_material = (ComponentMaterial*)cylinder->CreateComponent(component_type::Material);
	cylinder_material->diffuse_map = App->textures->loadTexture("Checkers_Texture.jpg", false);

	return cylinder;
}