#include "Component.h"

#include "Application.h"
#include "Globals.h"
#include "ModuleResources.h"
#include "ModuleScene.h"

#include "GameObject.h"

#include "imgui/imgui.h"

Component::Component(GameObject* go, component_type type) : my_go(go), type(type)
{
	uuid = App->resources->GenerateNewUID();
	my_go_uid = my_go->uuid;
}

Component::Component(const Component& comp)
{
	uuid = App->resources->GenerateNewUID();
	my_go_uid = comp.my_go_uid;
	my_go = comp.my_go;
	type = comp.type;
	active = comp.active;
}

Component::~Component()
{
	my_go = nullptr;
}

void Component::Update()
{
}

void Component::Enable() 
{
	active = true; 	
	App->scene->SetSceneDirty(true); 
}

void Component::Disable() 
{ 
	active = false; 
	App->scene->SetSceneDirty(true);
}

bool Component::DrawOnInspector()
{
	if(ImGui::Checkbox("Active", &active))
		App->scene->SetSceneDirty(true); 
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.8f, 0.8f));

	if (ImGui::SmallButton("Delete Component"))
	{
		Delete();
		ImGui::PopStyleColor(3);
		return true;
	}

	// Serialization information
	ImGui::Separator();
	ImGui::Text("UUID: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), uuid.c_str());
	ImGui::Text("GOUID: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), my_go_uid.c_str());
	ImGui::Separator();


	ImGui::PopStyleColor(3);
	return false;
}

void Component::Delete()
{
	my_go->DeleteComponent(*this);
}

unsigned Component::GetComponentNumber() const
{
	if (my_go == nullptr)
	{
		LOG("Warning: this component isn't attached to any GameObject.");
		return -1;
	}
	auto pos = std::find(my_go->components.begin(), my_go->components.end(), this) - my_go->components.begin();
	if (pos >= my_go->components.size())
	{
		//LOG("Warning: component not found as a component of %s.", my_go->name);
		return -1;
	}
	return pos;
}

JSON_value* Component::Save(JSON_value* component) const
{
	JSON_value* comp = component->createValue();
	comp->AddString("UID", uuid.c_str());
	comp->AddString("GOUID", my_go_uid.c_str());
	comp->AddInt("Type", (int)type);
	comp->AddBool("Active", active);

	return comp;
}

void Component::Load(JSON_value* component)
{
	uuid = component->GetString("UID");
	my_go_uid = component->GetString("GOUID");
	active = component->GetBool("Active");
}