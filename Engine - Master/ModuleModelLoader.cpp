#include "ModuleModelLoader.h"

#include "Application.h"
#include "ModuleScene.h"

#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#include "GameObject.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "par_shapes.h"
#pragma warning(pop)

ModuleModelLoader::ModuleModelLoader()
{
}

ModuleModelLoader::~ModuleModelLoader()
{
}

void ModuleModelLoader::CreateGameObjectFromFile(const char& name)
{
	GameObject* go = App->scene->CreateGameObject(&name);
	//Add Mesh Component
	ComponentMesh* mesh_comp = (ComponentMesh*)go->CreateComponent(component_type::Mesh);
	mesh_comp->LoadMesh(&name);
}

void ModuleModelLoader::LoadGeometry(GameObject* parent, Geometry_type geometryType) 
{
	par_shapes_mesh_s* parMesh = nullptr;

	switch (geometryType) 
	{
	case Geometry_type::SPHERE:
		parMesh = par_shapes_create_parametric_sphere(30, 30);
		break;
	case Geometry_type::TORUS:
		parMesh = par_shapes_create_torus(30, 30, 0.5f);
		break;
	case Geometry_type::PLANE:
		parMesh = par_shapes_create_plane(30, 30);
		break;
	case Geometry_type::CUBE:
		parMesh = par_shapes_create_cube();
		break;
	}

	if (parMesh != nullptr) 
	{
		par_shapes_scale(parMesh, 100.0f, 100.0f, 100.0f);

		ComponentMesh* mesh = (ComponentMesh*)parent->CreateComponent(component_type::Mesh);
		mesh->ComputeMesh(parMesh);
		par_shapes_free_mesh(parMesh);

		ComponentMaterial* mat = (ComponentMaterial*)parent->CreateComponent(component_type::Material);
		parent->ComputeBBox();
	}
	else
	{
		LOG("Error: error loading par_shapes mesh");
	}
}