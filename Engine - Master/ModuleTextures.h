#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include "Module.h"

#include <string>

enum class MaterialType;


class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();

	void			LoadTexture(std::string path, unsigned& textureID, int& width, int& height);
	void			Unload(unsigned id);
};

#endif /*__MODULETEXTURES_H__*/