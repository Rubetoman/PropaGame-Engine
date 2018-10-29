#pragma once
#include "Module.h"
#include "Globals.h"
#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilut.h>

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();
	bool CleanUp();
	GLuint loadImage(const char* theFileName);
};