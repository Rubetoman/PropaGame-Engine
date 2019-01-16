#ifndef __MODULESHADER_H__
#define __MODULESHADER_H__

#include "Module.h"

#include "GL/glew.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleShader : public Module
{
public:
	ModuleShader();
	~ModuleShader();

	bool Init();

	char*	ReadShaderFile(const char* shaderPath);
	GLuint	LoadShaders(const char* vertShaderPath, const char* fragShaderPath);
	void	CheckCompilationErrors(GLuint shader);

public:
	enum Program
	{
		DEFAULT = 0,
		FLAT,
		GOURAUD,
		PHONG,
		BLINN,
		PROGRAM_COUNT
	};

	unsigned programs[PROGRAM_COUNT];
};

#endif /* __MODULESHADER_H__ */

