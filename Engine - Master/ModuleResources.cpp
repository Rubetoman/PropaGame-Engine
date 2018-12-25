#include "ModuleResources.h"

#include "ModuleTextures.h"
ModuleResources::ModuleResources()
{
}


ModuleResources::~ModuleResources()
{
	App->textures->unloadTexture(checkers_texture);
	checkers_texture = nullptr;
	App->textures->unloadTexture(no_camera_texture);
	no_camera_texture = nullptr;
}

bool ModuleResources::Init()
{
	checkers_texture = App->textures->loadTexture(CHECKERS_TEXTURE);
	no_camera_texture = App->textures->loadTexture(CAMERA_TEXTURE);

	if (checkers_texture == nullptr || no_camera_texture == nullptr)
		return false;

	return true;
}

bool ModuleResources::CleanUp()
{
	textures.clear();
	lights.clear();

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