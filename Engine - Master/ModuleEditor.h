#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include "GL/glew.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include <vector> 

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
	void HandleInputs(SDL_Event& event);
	void ShowPerformanceWindow();
	void ShowTexturesWindow();

public:
	ImGuiIO io;
	//The window we'll be rendering to
	SDL_Window * window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

	// Performance data
	std::vector<float> fps_log;
	std::vector<float> ms_log;

	bool show_options_window = false;
	bool show_log_window = false;
	bool show_about_window = false;
	bool show_performance_window = false;
	bool show_textures_window = false;
};

#endif // __ModuleEditor_H__

