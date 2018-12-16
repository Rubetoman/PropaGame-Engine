#include "ComponentLight.h"

#include "Application.h"
#include "ModuleResources.h"

ComponentLight::ComponentLight(GameObject* go) : Component(go, component_type::Light)
{
	App->resources->lights.push_back(my_go);
}

ComponentLight::ComponentLight(const ComponentLight& comp) : Component(comp)
{
	intensity = comp.intensity;
}

ComponentLight::~ComponentLight()
{
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

void ComponentLight::Save(JSON_value* component) const
{
	Component::Save(component);

	JSON_value* light = component->createValue();

	light->AddInt("intensity", intensity);
	component->addValue("", light);
}

void ComponentLight::Load(JSON_value* component)
{
	Component::Load(component);
	
	intensity = component->GetInt("intensity");
}