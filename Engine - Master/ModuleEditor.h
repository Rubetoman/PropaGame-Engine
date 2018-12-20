#ifndef __MODULEEDITOR_H__
#define __MODULEEDITOR_H__

#include "Module.h"
#include "Window.h"
#include "ModuleModelLoader.h"
#include "ModuleTextures.h"
#include "Texture.h"

#include "Window.h"
#include "WindowScene.h"
#include "WindowAbout.h"
#include "WindowConsole.h"
#include "WindowHardware.h"
#include "WindowConfiguration.h"
#include "WindowHierarchy.h"
#include "WindowInspector.h"
#include "ModuleCamera.h"

#include "SDL/include/SDL.h"
#include "GL/glew.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "Assimp/include/assimp/version.h"
#include <vector> 

class Application;
class WindowScene;
class WindowAbout;
class WindowConsole;
class WindowHardware;
class WindowConfiguration;
class WindowHierarchy;
class WindowInspector;
class WindowCamera;

class ModuleEditor : public Module
{
public:

	ModuleEditor();
	virtual ~ModuleEditor();

	bool			Init();
	update_status	PreUpdate();
	update_status	Update();
	bool			CleanUp();

	// ImGui
	void Draw();
	void DrawDebugReferences();
	void CreateDockSpace() const;
	void ShowMainMenuBar();
	void SceneSavePopup();
	void SceneLoadPopup();
	void ShowInBrowser(const char* url) const;

	// SDL
	void HandleInputs(SDL_Event& event);

public:

	update_status update = UPDATE_CONTINUE;

	bool show_grid = true;
	bool show_axis = true;
	bool drawAllBBox = false;

	//The window we'll be rendering to
	SDL_Window * window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	//Docking windows
	WindowScene* scene = nullptr;
	WindowAbout* about = nullptr;
	WindowConsole* console = nullptr;
	WindowHardware* hardware = nullptr;
	WindowConfiguration* configuration = nullptr;
	WindowHierarchy* hierarchy = nullptr;
	WindowInspector* inspector = nullptr;
	WindowCamera* camera = nullptr;
	//Pop ups
	bool show_scene_save_popup = false;
	bool show_scene_load_popup = false;

private:
	std::list<Window*> editorWindows;
	char temp_name[64] = "";
};

#endif // __MODULEEDITOR_H__

