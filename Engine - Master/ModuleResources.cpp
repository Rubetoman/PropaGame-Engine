#include "ModuleResources.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleEditor.h"
#include "ModuleFileManager.h"

#include "GameObject.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ComponentLight.h"

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
	fbx_meshes = new std::vector<std::string>();
	external_textures = new std::vector<std::string>();

	// Assign textures used by the Engine
	App->textures->LoadTexture("Checkers_Texture.proDDS", checkers_texture, (int&)checkers_size.x, (int&)checkers_size.y);
	App->textures->LoadTexture("No_Camera.proDDS", no_camera_texture, (int&)no_camera_size.x, (int&)no_camera_size.y);

	UpdateFilesList();

	return true;
}

update_status ModuleResources::Update()
{
	// Check if lists need to update
	if (meshes_dirty)
	{
		App->resources->UpdateMeshesList();
		meshes_dirty = false;
	}
	if (textures_dirty)
	{
		App->resources->UpdateTexturesList();
		textures_dirty = false;
	}
	if (scenes_dirty)
	{
		App->resources->UpdateScenesList();
		scenes_dirty = false;
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

ComponentLight* ModuleResources::GetDirectionalLight() const
{
	for (const auto& light : lights)
	{
		if (light->type == light_type::Directional && light->my_go->active)
		{
			return light;
		}
	}
	return nullptr;
}

unsigned ModuleResources::GetLightNumber(ComponentLight& light) const
{
	auto pos = std::find(lights.begin(), lights.end(), &light) - lights.begin();
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
		{
			if (mesh->currentMesh.size() < 1)	
				{LOG("No mesh was selected on mesh component.");}
			else								
				{LOG("Mesh wasn't stored on resources mesh list.");}
		}
			
	}
	else
		LOG("Warning: Component was nullptr.");
}

void ModuleResources::UpdateFilesList()
{
	// External files
	UpdateFBXMeshesList();
	UpdateExternalTexturesList();

	// Own created files
	UpdateMeshesList();
	UpdateTexturesList();
	UpdateScenesList();
}

void ModuleResources::UpdateFBXMeshesList()
{
	if (fbx_meshes != nullptr)
	{
		fbx_meshes->clear();
		App->file->GetFilesFromDirectory(MODELS_ASSETS_FOLDER, *fbx_meshes);
	}
}

void ModuleResources::UpdateExternalTexturesList()
{
	if (external_textures != nullptr)
	{
		external_textures->clear();
		App->file->GetFilesFromDirectory(TEXTURES_ASSETS_FOLDER, *external_textures);
	}
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