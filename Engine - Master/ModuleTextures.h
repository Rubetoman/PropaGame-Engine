#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include "Module.h"
#include "Globals.h"
#include "Texture.h"
#include "ModuleFileManager.h"

#include "ComponentMaterial.h"

#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilut.h>
//#include <map>

enum class MaterialType;


class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();
	bool CleanUp();
	//Texture* loadTexture(const char* path, bool permanent); // Permanent boolean avoids deleting the texture when no in use
	//bool unloadTexture(Texture* texture);
	//void ReloadTexture(Texture& new_texture, GLuint& texture);

	void			LoadMaterial(std::string path, unsigned& textureID, int& width, int& height);
	void			LoadMaterial(const char* path, ComponentMaterial* componentMaterial, MaterialType materialTypeSelected);
	void			Unload(unsigned id);
};

#endif /*__MODULETEXTURES_H__*/