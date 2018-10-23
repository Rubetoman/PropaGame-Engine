#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui/imgui.h"
//#include "imgui/imgui_impl_opengl3.h"
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
	void* context;
};
