#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include "GL/glew.h"
#include "ModuleCamera.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	update_status Update();
	bool CleanUp();

	void ToggleFullScreen();
	void ToggleResizable();
	void ToggleVSync();
	void SetWindowSize(int w, int h, bool useSDL);

	int screen_width = SCREEN_WIDTH;
	int screen_height = SCREEN_HEIGHT;
	bool fullscreen = FULLSCREEN;
	bool vsync = VSYNC;
	bool resizable = RESIZABLE;

	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

	SDL_DisplayMode DM;
};

#endif // __ModuleWindow_H__