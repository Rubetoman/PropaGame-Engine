#include "WindowHierarchy.h"

#include "Application.h"
#include "ModuleScene.h"
#include "ModuleInput.h"

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
			// GO Management
			if (selected != nullptr)
			{
				ImGui::Separator();
				if (ImGui::Selectable("Copy"))
				{

				}
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
			}
			if (ImGui::Selectable("Paste"))
			{

			}
			// GO Creation
			ImGui::Separator();
			if (ImGui::Selectable("Create Empty"))
			{
				if (selected != nullptr)
					selected = App->scene->CreateGameObject(GO_DEFAULT_NAME, selected);
				else
					selected = App->scene->CreateGameObject(GO_DEFAULT_NAME);
			}
			if (ImGui::Selectable("Create Sphere"))
			{
				if (selected != nullptr)
					selected = App->scene->CreateSphere(1u, 20u, 20u, selected);
				else
					App->model_loader->CreateSphere("sphere", math::float3(0.0f, 0.0f, 0.0f), Quat::identity, math::float3(1.0f, 1.0f, 1.0f), 20, 20, float4(0.f, 0.0f, 0.5f, 1.0f));
			}
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

void WindowHierarchy::DrawNode()
{
	if (App->scene->show_scene_gos)
	{
		for (auto &go : App->scene->scene_gos)
		{
			ImGui::PushID(go);

			// Show with different color when it is a inactive GO
			if (!go->active)
				ImGui::PushStyleColor(ImGuiCol_Text, { 0.1f,0.1f,0.1f,0.9f });

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			flags |= go == selected ? ImGuiTreeNodeFlags_Selected : 0;

			//Check for children
			if (go->children.empty())
			{
				flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
			}

			bool node_open = ImGui::TreeNodeEx(go->name.c_str(), flags);

			if (ImGui::IsItemClicked() || (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)))
			{
				selected = go;
			}

			if (node_open)
			{
				// Children
				for (auto &child : go->children)
				{
					DrawChildren(child);
				}
				if (!(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
				{
					ImGui::TreePop();
				}
			}
			if (!go->active)
				ImGui::PopStyleColor();

			ImGui::PopID();
		}	
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
	ImGui::PushID(node);

	// Show with different color when it is a inactive GO
	if (!node->active)
		ImGui::PushStyleColor(ImGuiCol_Text, { 0.1f,0.1f,0.1f,0.9f });

	// Flags
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
		| (node == selected ? ImGuiTreeNodeFlags_Selected : 0);
	if (node->children.empty())
	{
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}
	bool node_open = ImGui::TreeNodeEx(node->name.c_str(), flags);

	// Drag and drop
	SetDragAndDrop(node);

	// Select input
	if (ImGui::IsItemClicked() || (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)))
	{
		selected = node;
	}

	// Draw children
	if (node_open)
	{
		for (auto &child : node->children)
		{
			DrawChildren(child);
		}

		if (!(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
		{
			ImGui::TreePop();
		}
	}

	if (!node->active)
		ImGui::PopStyleColor();

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