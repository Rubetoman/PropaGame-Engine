#include "ModuleResources.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleEditor.h"

#include "GameObject.h"
#include "Texture.h"
#include "ComponentCamera.h"

#include "TextureImporter.h"

ModuleResources::ModuleResources()
{
}

ModuleResources::~ModuleResources()
{
	if (checkers_texture != 0u)
		App->textures->Unload(checkers_texture);

	if (no_camera_texture != 0u)
		App->textures->Unload(no_camera_texture);
}

bool ModuleResources::Init()
{
	file_meshes = new std::vector<std::string>();
	file_textures = new std::vector<std::string>();
	file_scenes = new std::vector<std::string>();

//	TextureImporter::Import(CHECKERS_TEXTURE);
//	App->textures->LoadTexture("Checkers_Texture.proDDS", checkers_texture, (int&)checkers_size.x, (int&)checkers_size.y);

//	TextureImporter::Import(CAMERA_TEXTURE);
//	App->textures->LoadTexture("No_Camera.proDDS", no_camera_texture, (int&)no_camera_size.x, (int&)no_camera_size.y);

//	TextureImporter::Import("Assets/Models/textures/Baker_house.png");

	UpdateMeshesList();
	UpdateTexturesList();
	UpdateScenesList();

	return true;
}

update_status ModuleResources::Update()
{
	if (itemToDelete)
	{
		itemToDelete = false;
		if (removeMesh)
			UpdateMeshesList();
		else
			UpdateTexturesList();
	}
	return UPDATE_CONTINUE;
}

bool ModuleResources::CleanUp()
{
	file_meshes->clear();
	file_textures->clear();
	file_scenes->clear();
	lights.clear();
	meshes.clear();

	return true;
}

std::string ModuleResources::GenerateNewUID()
{
	return xg::newGuid().str();
}

unsigned ModuleResources::GetLightNumber(GameObject& go) const
{
	auto pos = std::find(lights.begin(), lights.end(), &go) - lights.begin();
	if (pos >= lights.size())
	{
		LOG("Warning: go not found as a resource of lights.");
		return -1;
	}
	return pos;
}

unsigned ModuleResources::GetCameraNumber(ComponentCamera& camera) const
{
	auto pos = std::find(cameras.begin(), cameras.end(), &camera) - cameras.begin();
	if (pos >= cameras.size())
	{
		LOG("Warning: %s not found as a camera on list of cameras.", camera.my_go->name);
		return -1;
	}
	return pos;
}

void ModuleResources::DeleteCamera(ComponentCamera* camera)
{
	if (camera != nullptr)
	{
		// Delete Window
		App->editor->DeleteCameraWindow(camera->window);

		int position = GetCameraNumber(*camera);
		if (position > -1)
		{
			cameras.erase(cameras.begin() + position);
			camera->CleanUp();
			//RELEASE(camera);
		}
		else
			LOG("Error deleting component.");
	}
	else
		LOG("Warning: Component was nullptr.");
}

unsigned ModuleResources::GetMeshNumber(ComponentMesh& mesh) const
{
	auto pos = std::find(meshes.begin(), meshes.end(), &mesh) - meshes.begin();
	if (pos >= meshes.size())
	{
		LOG("Warning: mesh not found as a resource of meshes.");
		return -1;
	}
	return pos;
}

void ModuleResources::DeleteMesh(ComponentMesh* mesh)
{
	if (mesh != nullptr)
	{
		int position = GetMeshNumber(*mesh);
		if (position > -1)
		{
			meshes.erase(meshes.begin() + position);
			//RELEASE(mesh);
		}
		else
			LOG("Error deleting component.");
	}
	else
		LOG("Warning: Component was nullptr.");
}

void ModuleResources::UpdateMeshesList()
{
	if (file_meshes != nullptr)
	{
		file_meshes->clear();
		App->file->GetFilesFromDirectory(MESHES_FOLDER, *file_meshes);
	}
}

void ModuleResources::UpdateTexturesList()
{
	if (file_textures != nullptr)
	{
		file_textures->clear();
		App->file->GetFilesFromDirectory(TEXTURES_FOLDER, *file_textures);
	}
}

void ModuleResources::UpdateScenesList()
{
	if (file_scenes != nullptr)
	{
		file_scenes->clear();
		App->file->GetFilesFromDirectory(SCENES_FOLDER, *file_scenes);
	}
}