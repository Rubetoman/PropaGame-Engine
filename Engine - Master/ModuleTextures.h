#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include "Module.h"
#include "Globals.h"
#include "Texture.h"
#include "ModuleFileManager.h"
#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilut.h>
//#include <map>

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();
	bool CleanUp();
	Texture* loadTexture(const char* path, bool permanent); // Permanent boolean avoids deleting the texture when no in use
	bool unloadTexture(Texture* texture);
	//void ReloadTexture(Texture& new_texture, GLuint& texture);
};

#endif /*__MODULETEXTURES_H__*/