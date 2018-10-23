#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include "GL/glew.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"

class Application;

class ModuleEditor : public Module
{
public:

	ModuleEditor();

	// Destructor
	virtual ~ModuleEditor();

	// Called before quitting
	bool Init();

	// Called before quitting
	update_status Update();

	// Called before quitting
	bool CleanUp();

public:
	//The window we'll be rendering to
	SDL_Window * window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;
};

#endif // __ModuleEditor_H__

