#include "WindowInspector.h"

#include "Application.h"
#include "Component.h"
#include "ModuleEditor.h"

#include "GameObject.h"
#include <vector>

WindowInspector::WindowInspector(const char* name) : Window(name)
{
	active = true;
}


WindowInspector::~WindowInspector()
{
}

void WindowInspector::Draw()
{
	if (ImGui::Begin("Inspector", &active, ImGuiWindowFlags_NoFocusOnAppearing))
	{
		GameObject* go = App->editor->hierarchy->selected;
		if (go != nullptr)
		{
			ImGui::Checkbox("active", &go->active);
			ImGui::SameLine();
			// Show name field
			char *name = new char[GO_NAME_SIZE];
			strcpy(name, go->name.c_str());
			ImGui::InputText("Name", name, GO_NAME_SIZE);
			go->name = name;
			delete[] name;

			if (ImGui::Checkbox("static", &go->static_GO))
			{
				if(go->children.size() > 0)
					show_static_popup = true;
			}

			if (show_static_popup)
				StaticPopup(*go);

			// Serialization information
			ImGui::Separator();
			ImGui::Text("UUID: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), go->uuid.c_str());
			ImGui::Text("Parent UID: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), go->parentUID.c_str());
			ImGui::Separator();

			if (go->components.size() > 0)
			{
				DrawComponents(go);
			}
			else
			{
				ImGui::Text("No components attached.");
			}

			if (ImGui::Button("Add Component", ImVec2(ImGui::GetWindowWidth(), 25))) 
			{
				ImGui::OpenPopup("AddComponentPopup");
			}
			ImGui::SameLine();

			if (ImGui::BeginPopup("AddComponentPopup")) 
			{
				if (ImGui::Selectable("Mesh")) 
				{
					go->CreateComponent(component_type::Mesh);
				}
				ImGui::Separator();
				if (ImGui::Selectable("Camera")) 
				{
					go->CreateComponent(component_type::Camera);
				}
				ImGui::EndPopup();
			}
		}
	}
	ImGui::End();
}

void WindowInspector::DrawComponents(GameObject* go) const
{
	for (auto &comp : go->components)
	{
		if(comp != nullptr && go != nullptr)
			comp->DrawOnInspector();
	}
}

void WindowInspector::StaticPopup(GameObject& go)
{
	ImGui::OpenPopup("Static");
	if (ImGui::BeginPopupModal("Static", &show_static_popup))
	{
		ImGui::Text("Change static state of the children \nattached to this GameObject as well?");
		ImGui::NewLine();
		if (ImGui::Button("Yes", ImVec2(120, 0)))
		{
			go.SetChildrenStatic(go.static_GO);
			show_static_popup = false;
		}

		ImGui::SameLine();

		if (ImGui::Button("No", ImVec2(120, 0)))
			show_static_popup = false;

		ImGui::EndPopup();
	}
}