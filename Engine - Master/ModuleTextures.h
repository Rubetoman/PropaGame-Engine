#pragma once
#include "Module.h"
#include "Globals.h"
#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilut.h>

enum Resize_Mode {nearest, linear};
enum Wrap_Mode {clamp_to_boder, clamp, repeat, mirrored};

struct image {
	const char* path = "";
	int width = 0;
	int height = 0;
	char* format = "";
	Resize_Mode resize_mode = linear;
	Wrap_Mode wrap_mode = clamp;
	bool use_mipmap = false;
};
class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();
	bool CleanUp();
	GLuint loadImage(image image);
	void unloadImage(unsigned id);
};