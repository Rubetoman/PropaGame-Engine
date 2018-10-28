#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleEditor;
<<<<<<< HEAD
class ModuleCamera;
=======
class ModuleShader;
>>>>>>> master

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleInput* input = nullptr;
    ModuleRenderExercise* exercise = nullptr;
	ModuleEditor* editor = nullptr;
<<<<<<< HEAD
	ModuleCamera* camera = nullptr;
=======
	ModuleShader* shader = nullptr;
>>>>>>> master

private:

	std::list<Module*> modules;

};

extern Application* App;
