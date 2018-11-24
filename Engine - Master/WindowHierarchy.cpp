#include "WindowHierarchy.h"

#include "Application.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
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
				if (ImGui::IsItemHovered())
				{
					if (ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1))
					{
						selected = (*go);
					}
				}
				ImGui::TreePop();
			}
		}

		// Popup
		if (ImGui::IsMouseReleased(1) & ImGui::IsMouseHoveringWindow())
		{
			ImGui::OpenPopup("go_menu_popup");
		}
		if (ImGui::BeginPopup("go_menu_popup"))
		{
			if (ImGui::Selectable("Copy"))
			{

			}
			if (ImGui::Selectable("Paste"))
			{

			}
			ImGui::Separator();
			if (selected != nullptr)
			{
				if (ImGui::Selectable("Duplicate"))
				{

				}
				if (ImGui::Selectable("Delete"))
				{

				}
				ImGui::Separator();
			}
			if (ImGui::Selectable("Create Empty"))
			{
				App->scene->CreateGameObject("GameObject");
			}
			ImGui::EndPopup();
		}
		

	}
	ImGui::End();
}