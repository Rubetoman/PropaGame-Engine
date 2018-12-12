#include "ComponentLight.h"



ComponentLight::ComponentLight(GameObject* go) : Component(go, component_type::Light)
{
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