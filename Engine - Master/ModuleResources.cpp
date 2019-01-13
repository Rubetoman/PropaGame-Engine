#include "ModuleResources.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleEditor.h"

#include "GameObject.h"
#include "Texture.h"
#include "ComponentCamera.h"

#include "MaterialImporter.h"

ModuleResources::ModuleResources()
{
}

ModuleResources::~ModuleResources()
{
	//App->textures->unloadTexture(checkers_texture);
	checkers_texture = nullptr;
	//App->textures->unloadTexture(no_camera_texture);
	no_camera_texture = nullptr;
}

bool ModuleResources::Init()
{
	file_meshes = new std::vector<std::string>();
	file_textures = new std::vector<std::string>();
	MaterialImporter::Import("Assets/Models/textures/Baker_house.png");
	//checkers_texture = App->textures->loadTexture(CHECKERS_TEXTURE, true);
	//no_camera_texture = App->textures->loadTexture(CAMERA_TEXTURE, true);

	UpdateMeshesList();
	UpdateTexturesList();
	/*if (checkers_texture == nullptr || no_camera_texture == nullptr)
		return false;*/

	return true;
}

bool ModuleResources::CleanUp()
{
	textures.clear();
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
	file_meshes->clear();
	App->file->GetFilesFromDirectory(MESHES_FOLDER, *file_meshes);
}

void ModuleResources::UpdateTexturesList()
{
	file_textures->clear();
	App->file->GetFilesFromDirectory(TEXTURES_FOLDER, *file_textures);
}