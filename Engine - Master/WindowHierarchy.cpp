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
		GameObject* node = App->scene->root;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		flags |= node == selected ? ImGuiTreeNodeFlags_Selected : 0;

		if (ImGui::TreeNodeEx(node->name.c_str(), flags))
		{
			if (ImGui::IsItemClicked() ||(ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)))
			{
				selected = node;
			}

			// Children
			for (auto &child : node->children)
			{
				DrawChildren(child);
			}
			ImGui::TreePop();
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
				if (ImGui::Selectable("Create Empty"))
				{
					App->scene->CreateGameObject(GO_DEFAULT_NAME, selected);
				}
			}
			else
			{
				if (ImGui::Selectable("Create Empty"))
				{
					App->scene->CreateGameObject(GO_DEFAULT_NAME);
				}
			}
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

void WindowHierarchy::DrawChildren(GameObject* node)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	if (node->children.empty())
	{
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}
	flags |= node == selected ? ImGuiTreeNodeFlags_Selected : 0;

	if (ImGui::TreeNodeEx(node->name.c_str(), flags))
	{
		if (ImGui::IsItemClicked() || (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)))
		{
			selected = node;
		}

		// Children
		for (auto &child : node->children)
		{
			DrawChildren(child);
		}

		if (!(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
		{
			ImGui::TreePop();
		}
	}
}