#include "WindowHierarchy.h"

#include "Application.h"
#include "ModuleScene.h"
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
	if (ImGui::Begin("Hierarchy", &active, ImGuiWindowFlags_NoFocusOnAppearing))
	{
		
		for (std::vector<GameObject*>::iterator go = App->scene->game_objects.begin(); go != App->scene->game_objects.end(); ++go)
		{
			unsigned flags = ImGuiTreeNodeFlags_Leaf;
			flags |= (*go) == selected ? ImGuiTreeNodeFlags_Selected : 0;

			if (ImGui::TreeNodeEx((*go)->name.c_str(), flags))
			{
				if (ImGui::IsItemHoveredRect())
				{
					if (ImGui::IsMouseClicked(0))
					{
						selected = (*go);
					}
				}
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();
}