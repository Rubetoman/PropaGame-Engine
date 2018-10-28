#ifndef MODULESHADER_H
#define MODULESHADER_H

#include "Module.h"
#include "Globals.h"
#include "GL/glew.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleShader : public Module
{
public:
	ModuleShader();
	~ModuleShader();
	char*	ReadShaderFile(const char* shaderPath);
	GLuint	LoadShaders(const char* vertShaderPath, const char* fragShaderPath);
	void	CheckCompilationErrors(GLuint shader);
};

#endif

