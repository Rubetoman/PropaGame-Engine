#include "ComponentLight.h"

#include "Application.h"
#include "ModuleResources.h"
#include "ModuleScene.h"

#include "GameObject.h"
#include "ComponentTransform.h"

#include "imgui/imgui.h"

ComponentLight::ComponentLight(GameObject* go) : Component(go, component_type::Light)
{
}

ComponentLight::ComponentLight(const ComponentLight& comp) : Component(comp)
{
	type = comp.type;
	direction = comp.direction;
	color = comp.color;
	attenuation = comp.attenuation;
}

ComponentLight::~ComponentLight()
{
	unsigned pos = App->resources->GetLightNumber(*my_go);
	if (pos > -1)
		App->resources->lights.erase(App->resources->lights.begin() + pos);
}

Component* ComponentLight::Duplicate()
{
	App->scene->SetSceneDirty(true);
	return new ComponentLight(*this);
}

void ComponentLight::Update()
{
	direction = -(my_go->transform->rotation*float3::unitZ).Normalized();
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
			const char* light_types[(int)light_type::Light_Count] = { "Point", "Directional", "Spot" };
			if (ImGui::Combo("Type", (int*)&type, light_types, (int)light_type::Light_Count))
				App->scene->SetSceneDirty(true);

			if (ImGui::ColorEdit3("Color", (float*)&color)) { App->scene->SetSceneDirty(true); }

			if (type != light_type::Directional)
			{
				ImGui::Text("Attenuation");
				if (ImGui::DragFloat("Constant", (float*)&attenuation.x, 0.01f, 0.001f, 10.f)) { App->scene->SetSceneDirty(true); }
				if (ImGui::DragFloat("Linear", (float*)&attenuation.y, 0.0001f, 0.0001f, 1.f)) { App->scene->SetSceneDirty(true); }
				if (ImGui::DragFloat("Quadratic", (float*)&attenuation.z, 0.0001f, 0.0f, 1.f)) { App->scene->SetSceneDirty(true); }
			}

			if (type == light_type::Spot)
			{
				ImGui::Text("Cutoff Angle:"); ImGui::NewLine();

				if (ImGui::DragFloat("Inner", (float*)&inner_cutoff, 0.1f, 0.f, 90.f)) { App->scene->SetSceneDirty(true); }
				if (ImGui::DragFloat("Outer", (float*)&outer_cutoff, 0.1f, 0.f, 90.f)) { App->scene->SetSceneDirty(true); }
			}
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

float ComponentLight::GetAttenuationDistance() const
{
	float a = attenuation[2];
	float b = attenuation[1];
	float c = attenuation[0] - 2.5f; // 1/(constant+linear*distance...) < 0.4

	float delta = b * b - 4 * a * c;
	if (delta < 0)
	{
		LOG("Error in Attenuation Distance");
		return 0.f;
	}

	if (a == 0) //quadratic is 0
	{
		return -c / b;
	}
	return (-b + sqrt(delta)) / (2 * a);

}

JSON_value* ComponentLight::Save(JSON_value* component) const
{
	JSON_value* light = Component::Save(component);

	//TODO:: Add save
	/*light->AddFloat("intensity", attenuation);
	component->addValue("", light);*/

	return light;
}

void ComponentLight::Load(JSON_value* component)
{
	Component::Load(component);
	
	//attenuation = component->GetFloat("intensity");
}