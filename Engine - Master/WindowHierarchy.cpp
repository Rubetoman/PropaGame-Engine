#include "WindowHierarchy.h"

#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include <vector>
#include <string>

WindowHierarchy::WindowHierarchy(const char* name) : Window(name)
{
	active = true;
}


WindowHierarchy::~WindowHierarchy()
{
}

void WindowHierarchy::Draw()
{
	if (ImGui::Begin("Hierarchy"))
	{
		
		for (std::vector<GameObject*>::iterator go = App->scene->game_objects.begin(); go != App->scene->game_objects.end(); ++go)
		{
			ImGui::Text((*go)->name.c_str());
		}
	}
	ImGui::End();
}