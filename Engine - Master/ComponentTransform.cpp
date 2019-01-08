#include "ComponentTransform.h"

#include "Application.h"
#include "ModuleScene.h"

#include "GameObject.h"

ComponentTransform::ComponentTransform(GameObject* go) : Component(go, component_type::Transform)
{
}

ComponentTransform::ComponentTransform(const ComponentTransform& comp) : Component(comp)
{
	position = comp.position;
	euler_rotation = comp.euler_rotation;
	rotation = comp.rotation;
	scale = comp.scale;
}

ComponentTransform::~ComponentTransform()
{
}

Component* ComponentTransform::Duplicate()
{
	return new ComponentTransform(*this);
}

void ComponentTransform::SetTransform(const math::float4x4& transform)
{
	math::float3 new_position;
	math::float3 new_scale;
	math::Quat new_rotation;
	transform.Decompose(new_position, new_rotation, new_scale);

	position = { new_position.x, new_position.y, new_position.z };
	scale = { new_scale.x, new_scale.y, new_scale.z };
	rotation = new_rotation;
	euler_rotation = rotation.ToEulerXYZ();
	euler_rotation.x = math::RadToDeg(euler_rotation.x);
	euler_rotation.y = math::RadToDeg(euler_rotation.y);
	euler_rotation.z = math::RadToDeg(euler_rotation.z);
}

void ComponentTransform::SetTransform(const math::float3& pos, const math::Quat& rot, const math::float3& sca)
{
	position = pos;
	scale = sca;
	rotation = rot;
	euler_rotation = rotation.ToEulerXYZ();
	euler_rotation.x = math::RadToDeg(euler_rotation.x);
	euler_rotation.y = math::RadToDeg(euler_rotation.y);
	euler_rotation.z = math::RadToDeg(euler_rotation.z);
}

void ComponentTransform::SetRotation(const math::Quat& rot)
{
	rotation = rot;
	euler_rotation = rotation.ToEulerXYZ();
	euler_rotation.x = math::RadToDeg(euler_rotation.x);
	euler_rotation.y = math::RadToDeg(euler_rotation.y);
	euler_rotation.z = math::RadToDeg(euler_rotation.z);
}

void ComponentTransform::SetRotation(const math::float3& rot)
{
	euler_rotation = rot;
	rotation.FromEulerXYZ(euler_rotation.x, euler_rotation.y, euler_rotation.z);
}

void ComponentTransform::LocalToGlobal(const float4x4& global_transform)
{
	float4x4 global = global_transform;
	SetTransform(global);
}

void ComponentTransform::GlobalToLocal(const float4x4& local_transform)
{
	float4x4 global = float4x4::FromTRS(position, rotation, scale);
	float4x4 local = local_transform.Inverted() * global;
	SetTransform(local);
}

bool ComponentTransform::DrawOnInspector()
{
	ImGui::PushID(this);
	ImGui::Separator();

	if (ImGui::CollapsingHeader("Transform Component"))
	{
		// Serialization information
		ImGui::Text("UUID: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), uuid.c_str());
		ImGui::Text("GOUID: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), my_go_uid.c_str());
		ImGui::Separator();

		if(ImGui::DragFloat3("Position", (float*)&position, 0.1f))
		{
			if(my_go->static_GO)
				App->scene->dirty = true;
		}
		if (ImGui::DragFloat3("Rotation", (float*)&euler_rotation, 0.1f))
		{
			rotation = rotation.FromEulerXYZ(math::DegToRad(euler_rotation.x),
				math::DegToRad(euler_rotation.y), math::DegToRad(euler_rotation.z));
			if (my_go->static_GO)
				App->scene->dirty = true;
		}
		if(ImGui::DragFloat3("Scale", (float*)&scale, 0.1f))
		{
			if (my_go->static_GO)
				App->scene->dirty = true;
		}
	}
	ImGui::PopID();
	return false;
}

JSON_value* ComponentTransform::Save(JSON_value* component) const
{
	JSON_value* transform = Component::Save(component);

	transform->AddVec3("Position", position);
	transform->AddQuat("Rotation", rotation);
	transform->AddVec3("Scale", scale);

	component->addValue("", transform);

	return transform;
}	

void ComponentTransform::Load(JSON_value* component)
{
	Component::Load(component);

	SetTransform(component->GetVec3("Position"), component->GetQuat("Rotation"), component->GetVec3("Scale"));
}