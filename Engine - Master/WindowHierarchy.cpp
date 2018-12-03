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
		if (App->scene->root != nullptr)
			DrawNode();

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
					selected = App->scene->DuplicateGameObject(selected);
				}
				if ((selected->parent != App->scene->root) && (selected != App->scene->root))
				{
					if (ImGui::Selectable("Unchild"))
						App->scene->Unchild(selected);
				}
				if (ImGui::Selectable("Delete"))
				{
					selected->flags |= GOFlags::Delete;
					selected = nullptr;
				}
				ImGui::Separator();
				if (ImGui::Selectable("Create Empty"))
				{
					selected = App->scene->CreateGameObject(GO_DEFAULT_NAME, selected);
				}
			}
			else
			{
				if (ImGui::Selectable("Create Empty"))
				{
					selected = App->scene->CreateGameObject(GO_DEFAULT_NAME);
				}
			}
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

void WindowHierarchy::DrawNode()
{
	if (App->scene->show_root)
	{
		GameObject* node = App->scene->root;
		ImGui::PushID(node);
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		flags |= node == selected ? ImGuiTreeNodeFlags_Selected : 0;
		bool node_open = ImGui::TreeNodeEx(node->name.c_str(), flags);
		SetDragAndDrop(node);
		
		if (ImGui::IsItemClicked() || (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)))
		{
			selected = node;
		}

		if (node_open)
		{
			// Children
			for (auto &child : node->children)
			{
				DrawChildren(child);
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
	}
	else
	{
		for (auto &child : App->scene->root->children)
		{
			DrawChildren(child);
		}
	}
}

void WindowHierarchy::DrawChildren(GameObject* node)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
		| (node == selected ? ImGuiTreeNodeFlags_Selected : 0);
	ImGui::PushID(node);
	if (node->children.empty())
	{
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}
	bool node_open = ImGui::TreeNodeEx(node->name.c_str(), flags);
	SetDragAndDrop(node);

	if (ImGui::IsItemClicked() || (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)))
	{
		selected = node;
	}

	if (node_open)
	{
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
	ImGui::PopID();
}

void WindowHierarchy::SetDragAndDrop(GameObject* node)
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		GameObject* drag_go = node;
		ImGui::SetDragDropPayload("DragDropHierarchy", &drag_go, sizeof(GameObject*), ImGuiCond_Once);
		ImGui::Text("%s", node->name.c_str());
		ImGui::EndDragDropSource();
	}
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DragDropHierarchy"))
		{
			IM_ASSERT(payload->DataSize == sizeof(GameObject*));
			GameObject* drop_go = (GameObject*)*(const int*)payload->Data;
			if ((drop_go->parent != node) && !(node->isForefather(*drop_go)))
			{
				drop_go->SetParent(node);
			}
		}
		ImGui::EndDragDropTarget();
	}
}