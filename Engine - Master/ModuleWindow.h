#ifndef __MODULEWINDOW_H__
#define __MODULEWINDOW_H__

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
	bool CleanUp();

	void ToggleFullScreen();
	void ToggleResizable();
	void ToggleVSync();
	void ToggleBorderless();
	void SetWindowBrightness(int value);
	void SetWindowSize(unsigned& w, unsigned& h, bool useSDL);

	// Window configuration variables
	unsigned screen_width = SCREEN_WIDTH;
	unsigned screen_height = SCREEN_HEIGHT;
	bool fullscreen = FULLSCREEN;
	bool vsync = VSYNC;
	bool resizable = RESIZABLE;
	bool borderless = BORDERLESS;
	float brightness = BRIGHTNESS;

	unsigned previous_width = SCREEN_WIDTH;
	unsigned previous_height = SCREEN_HEIGHT;

	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	SDL_DisplayMode DM;
};

#endif /*__MODULEWINDOW_H__*/