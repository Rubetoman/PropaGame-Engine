#include "ModuleResources.h"

ModuleResources::ModuleResources()
{
}


ModuleResources::~ModuleResources()
{
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