#ifndef __MODULEEDITOR_H__
#define __MODULEEDITOR_H__

#include "Module.h"
#include "Window.h"
#include "ModuleModelLoader.h"
#include "ModuleTextures.h"
#include "Texture.h"
#include "WindowScene.h"
#include "SDL/include/SDL.h"
#include "GL/glew.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "Assimp/include/assimp/version.h"
#include <vector> 

class Application;
class WindowScene;

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
	void Draw();
	const void HandleInputs(SDL_Event& event);
	void CreateDockSpace();
	const void ShowAppInfoWindow();
	const void ShowPropertiesWindow();
	const void ShowConfigurationWindow();
	//void ShowTexturesWindow();
	const void ShowLogWindow();
	void AddLog(const char* logs);
	const void ShowInBrowser(const char* url) const;

public:
	ImGuiIO io;
	//The window we'll be rendering to
	SDL_Window * window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	// Performance data
	std::vector<float> fps_log;
	std::vector<float> ms_log;
	std::vector<float> mem_log;

	bool show_log_window = false;
	bool show_about_window = false;
	bool show_app_info_window = false;
	bool show_textures_window = false;
	bool show_properties_window = false;
	bool show_configuration_window = false;

	ImGuiTextBuffer Buffer;
	bool ScrollToBottom = false;

	update_status update = UPDATE_CONTINUE;

	//Docking windows
	WindowScene* scene = nullptr;

private:
	std::list<Window*> editorWindows;
};

#endif // __MODULEEDITOR_H__

