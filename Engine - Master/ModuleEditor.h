#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include "GL/glew.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"

class Application;

class ModuleEditor : public Module
{
public:

	ModuleEditor();

	// Destructor
	virtual ~ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

public:
	ImGuiIO io;
	//The window we'll be rendering to
	SDL_Window * window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

	bool show_options_window = false;
	bool show_log_window = false;
	bool show_about_window = false;
};

#endif // __ModuleEditor_H__

