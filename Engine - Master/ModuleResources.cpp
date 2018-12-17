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