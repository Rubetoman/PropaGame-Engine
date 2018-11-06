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

struct texture {
	const char* name = "";
	const char* path = "";
	int width = 0;
	int height = 0;
	char* format = "";
	Resize_Mode resize_mode = linear;
	Wrap_Mode wrap_mode = clamp;
	bool use_mipmap = false;
	const char* getResizeMode() {
		switch (this->resize_mode) {
		case nearest: return "nearest"; break;
		case linear: return "linear"; break;
		}
	}
	inline void setResizeMode(const char* mode) {
		if (mode == "nearest") {this->resize_mode = nearest;}
		else if (mode == "linear") { this->resize_mode = linear; }
	}
	const char* getWrapMode() {
		switch (this->wrap_mode) {
		case clamp_to_boder: return "clamp to boder"; break;
		case clamp: return "clamp"; break;
		case repeat: return "repeat"; break;
		case mirrored: return "mirrored"; break;
		}
	}
	inline void setWrapMode(const char* mode) {
		if (mode == "clamp to boder") { this->wrap_mode = clamp_to_boder; }
		else if (mode == "clamp") { this->wrap_mode = clamp; }
		else if (mode == "repeat") { this->wrap_mode = repeat; }
		else if (mode == "mirrored") { this->wrap_mode = mirrored; }
	}
};
class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();
	bool CleanUp();
	GLuint loadImage(texture& texture);
	void unloadImage(unsigned id);
	void ReloadImage(texture& new_texture, GLuint& texture);
};