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