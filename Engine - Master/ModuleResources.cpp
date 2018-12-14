#include "ModuleResources.h"

ModuleResources::ModuleResources()
{
}


ModuleResources::~ModuleResources()
{
}

std::string ModuleResources::GenerateNewUID()
{
	return xg::newGuid().str();
}