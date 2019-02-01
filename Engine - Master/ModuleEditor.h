#ifndef __MODULEEDITOR_H__
#define __MODULEEDITOR_H__

#include "Module.h"

#include "SDL_events.h"
#include "Math/float3.h"

#include <list>

class Application;

class Window;
class WindowScene;
class WindowAbout;
class WindowConsole;
class WindowHardware;
class WindowConfiguration;
class WindowHierarchy;
class WindowInspector;
class WindowCamera;
class WindowExplorer;

class GameObject;
class ComponentCamera;


enum class BBoxMode {Divide, All_Divide, Enclose, All_Enclose};

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

	// Cameras
	WindowCamera* CreateCameraWindow(ComponentCamera& camera);
	void DeleteCameraWindow(WindowCamera* camera);

public:

	update_status update = UPDATE_CONTINUE;

	// Selected GO
	GameObject* selectedGO = false;

	// Scale of the meshes
	float scale = 100.0f;

	// Debug draw
	bool show_grid = true;
	bool show_axis = true;
	bool show_raycast = false;
	math::float3 raycast_color = math::float3(0.0f, 1.0f, 0.0f);

	// Max number of light that a GO will render
	unsigned max_lights = 4u;

	BBoxMode bbox_mode = BBoxMode::Divide;

	//The window we'll be rendering to
	SDL_Window* window = nullptr;

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
	WindowExplorer* explorer = nullptr;

	//Pop ups
	bool show_scene_save_popup = false;
	bool show_scene_load_popup = false;

private:
	std::list<Window*> editorWindows;
	char temp_name[64] = "";
};

#endif // __MODULEEDITOR_H__

