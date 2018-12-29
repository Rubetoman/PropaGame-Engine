#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleWindow.h"
#include "ModuleEditor.h"
#include "ModuleShader.h"
#include "SDL.h"
#include "GL/glew.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void WindowResized(unsigned width, unsigned height);

	void SetViewMatrix(ComponentCamera* camera) const;
	void SetProjectionMatrix(ComponentCamera* camera) const;
	void GenerateBlockUniforms();

	void* context = nullptr;
	unsigned ubo = 0u;
};

#endif
