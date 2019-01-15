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
	//LoadMesh("Assets/Models/BakerHouse.fbx");
	//MeshImporter::ImportFBX("Assets/Models/BakerHouse.fbx");

	return true;
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

	// Generate root node
	aiMatrix4x4 root_transform = aiMatrix4x4() * scene->mRootNode->mTransformation;
	GameObject* root_go = App->scene->CreateGameObject(scene->mRootNode->mName.C_Str(), (math::float4x4&)root_transform, App->scene->root);

	// Generate meshes as root GOs
	GenerateNodeMeshData(path, scene, scene->mRootNode, root_transform, root_go);
	
	aiReleaseImport(scene);
	return true;
}

void ModuleModelLoader::GenerateNodeMeshData(const char* path, const aiScene* scene, const aiNode* node, const aiMatrix4x4& parent_transform, GameObject* parent)
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
		mesh->mesh_type = Mesh_type::fbx;
		mesh->currentMesh = path;

		const aiMesh* src_mesh = scene->mMeshes[node->mMeshes[i]];

		mesh->mesh.num_vertices = src_mesh->mNumVertices;
		mesh->mesh.num_indices = src_mesh->mNumFaces * 3;

		//Copying Vertices array
		mesh->mesh.vertices = new float[mesh->mesh.num_vertices * 3]; //It is checked below that at least has 1 face, so at least 3 vertices
		memcpy(mesh->mesh.vertices, src_mesh->mVertices, sizeof(float) * mesh->mesh.num_vertices * 3);

		//Copying Face Normals
		if (src_mesh->HasNormals())
		{
			mesh->mesh.normals = new float[mesh->mesh.num_vertices * 3];
			memcpy(mesh->mesh.normals, src_mesh->mNormals, sizeof(float)*mesh->mesh.num_vertices * 3);
		}

		// vertex array objects (VAO)
		glGenVertexArrays(1, &mesh->mesh.vao);
		glBindVertexArray(mesh->mesh.vao);

		glGenBuffers(1, &mesh->mesh.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->mesh.vbo);

		// Divide Buffer for position and UVs
		glBufferData(GL_ARRAY_BUFFER, mesh->mesh.num_vertices * (sizeof(float) * 3 + sizeof(float) * 2), nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * mesh->mesh.num_vertices, mesh->mesh.vertices);

		// Texture coords (UVs)
		// MapBufferRange because we only want UV data from UVW
		mesh->mesh.uvs = (float*)glMapBufferRange(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->mesh.num_vertices,
			sizeof(float) * 2 * mesh->mesh.num_vertices, GL_MAP_WRITE_BIT);

		int uvsCount = 0;
		for (unsigned i = 0u; i < mesh->mesh.num_vertices; i++)
		{
			mesh->mesh.uvs[uvsCount] = src_mesh->mTextureCoords[0][i].x;
			uvsCount++;
			mesh->mesh.uvs[uvsCount] = src_mesh->mTextureCoords[0][i].y;
			uvsCount++;
		}

		glUnmapBuffer(GL_ARRAY_BUFFER);

		// Indices (faces)
		glGenBuffers(1, &mesh->mesh.ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mesh.ibo);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->mesh.num_indices * (sizeof(unsigned)), nullptr, GL_STATIC_DRAW);

		mesh->mesh.indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned)*mesh->mesh.num_indices, GL_MAP_WRITE_BIT);

		for (unsigned j = 0; j < src_mesh->mNumFaces; ++j)
		{
			assert(src_mesh->mFaces[j].mNumIndices == 3);

			*(mesh->mesh.indices++) = src_mesh->mFaces[j].mIndices[0];
			*(mesh->mesh.indices++) = src_mesh->mFaces[j].mIndices[1];
			*(mesh->mesh.indices++) = src_mesh->mFaces[j].mIndices[2];
		}

		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * mesh->mesh.num_vertices));

		// Disable VAO
		glBindVertexArray(0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Disable VBO and EBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mesh->mesh.indices = new unsigned[mesh->mesh.num_indices]; // assume each face is a triangle
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
				memcpy(&mesh->mesh.indices[j * 3], src_mesh->mFaces[j].mIndices, 3 * sizeof(unsigned));
			}
		}

		mesh->mesh.boundingBox.SetNegativeInfinity();
		mesh->mesh.boundingBox.Enclose((math::float3*)src_mesh->mVertices, mesh->mesh.num_vertices);
		App->resources->meshes.push_back(mesh);
	}

	if (node->mNumMeshes == 0)
	{
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			GenerateNodeMeshData(path, scene, node->mChildren[i], parent_transform, parent);
		}
	}
}

/*GameObject* ModuleModelLoader::CreateSphere(const char* name, const math::float3& position, const math::Quat& rotation, const math::float3& scale,
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
}*/