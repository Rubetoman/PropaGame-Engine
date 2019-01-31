#include "ComponentLight.h"

#include "Application.h"
#include "ModuleResources.h"

#include "imgui/imgui.h"

ComponentLight::ComponentLight(GameObject* go) : Component(go, component_type::Light)
{
}

ComponentLight::ComponentLight(const ComponentLight& comp) : Component(comp)
{
	intensity = comp.intensity;
}

ComponentLight::~ComponentLight()
{
	unsigned pos = App->resources->GetLightNumber(*my_go);
	if (pos > -1)
		App->resources->lights.erase(App->resources->lights.begin() + pos);
}

Component* ComponentLight::Duplicate()
{
	return new ComponentLight(*this);
}

bool ComponentLight::DrawOnInspector()
{
	ImGui::PushID(this);
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Light Component"))
	{
		bool deleted = Component::DrawOnInspector();
		if (!deleted)
		{
			ImGui::SliderFloat("Intensity", &intensity, 0.0f, 1.0f);
		}
		else
		{
			ImGui::PopID();
			return deleted;
		}
	}
	ImGui::PopID();
	return false;
}

JSON_value* ComponentLight::Save(JSON_value* component) const
{
	JSON_value* light = Component::Save(component);

	light->AddFloat("intensity", intensity);
	component->addValue("", light);

	return light;
}

void ComponentLight::Load(JSON_value* component)
{
	Component::Load(component);
	
	intensity = component->GetFloat("intensity");
}