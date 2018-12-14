#include "WindowInspector.h"

#include "Application.h"
#include "Component.h"
#include "ModuleEditor.h"

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

			// Show name field
			char *name = new char[GO_NAME_SIZE];
			strcpy(name, go->name.c_str());
			ImGui::InputText("Name", name, GO_NAME_SIZE);
			go->name = name;
			delete[] name;

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
		}
	}
	ImGui::End();
}

void WindowInspector::DrawComponents(GameObject* go)
{
	for (auto &comp : go->components)
	{
		if(go != nullptr)
			comp->DrawOnInspector();
	}
}