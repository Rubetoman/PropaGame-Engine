#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleWindow.h"
#include "ModuleEditor.h"
#include "ModuleCamera.h"
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
	void CreateFrameBuffer();

	void* context = nullptr;

	GLuint program = 0;
	GLuint renderedTexture = 0;

	unsigned		fbo = 0u;
	unsigned		rbo = 0u;

	void			DrawCoordinates();
	void			DrawPlane();
};

#endif
