#pragma once
#include "Module.h"
#include "Globals.h"
#include "Texture.h"
#include "ModuleFileManager.h"
#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilut.h>
#include <vector>

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();
	bool CleanUp();
	GLuint loadTexture(const char* path);
	void unloadTexture(const unsigned& id);
	void ReloadTexture(Texture& new_texture, GLuint& texture);

	std::vector<Texture*> textures;
};