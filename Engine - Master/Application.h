#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleEditor;
class ModuleCamera;
class ModuleShader;
class ModuleModelLoader;
class ModuleFileManager;
class ModuleTime;

class Application
{
public:
	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void FinishUpdate();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleInput* input = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleShader* shader = nullptr;
	ModuleModelLoader* model_loader = nullptr;
	ModuleFileManager* file = nullptr;
	ModuleTime* time = nullptr;

private:

	std::list<Module*> modules;

};

extern Application* App;

#endif // __APPLICATION_H__