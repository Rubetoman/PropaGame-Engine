#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ModuleCamera.h"
#include "ModuleShader.h"
#include "ModuleModelLoader.h"
#include "ModuleFileManager.h"
#include "ModuleTime.h"
#include "ModuleDebugDraw.h"
#include "ModuleScene.h"
#include "ModuleResources.h"

using namespace std;

Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(time = new ModuleTime());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(window = new ModuleWindow());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(resources = new ModuleResources());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(input = new ModuleInput());
	modules.push_back(shader = new ModuleShader());
	modules.push_back(scene = new ModuleScene());
	modules.push_back(editor = new ModuleEditor());
	modules.push_back(file = new ModuleFileManager());
	modules.push_back(model_loader = new ModuleModelLoader());
	modules.push_back(debug_draw = new ModuleDebugDraw());
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	FinishUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}

void Application::FinishUpdate()
{
	int ms_cap = 1000 / time->max_fps;
	if (time->frame_timer.Read() < ms_cap)
		SDL_Delay(ms_cap - time->frame_timer.Read());
}

#endif /*__APPLICATION_CPP__*/
