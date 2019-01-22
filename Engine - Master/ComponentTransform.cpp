#include "ComponentTransform.h"

#include "Application.h"
#include "ModuleScene.h"
#include "ModuleEditor.h"

#include "GameObject.h"

#include "imgui/imgui.h"

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
	my_go->transform = nullptr;
}

Component* ComponentTransform::Duplicate()
{
	return new ComponentTransform(*this);
}

void ComponentTransform::SetTransform(const math::float4x4& transform)
{
	transform.Decompose(position, rotation, scale);

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

void ComponentTransform::SetGlobalTransform(const math::float4x4& global_transform)
{
	LocalToGlobal(global_transform);
	math::float4x4 parent_global = math::float4x4::identity;

	if (my_go->parent != nullptr && my_go->parent->transform != nullptr)
	{
		parent_global = my_go->parent->GetGlobalTransform();
	}
	GlobalToLocal(parent_global);
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

void ComponentTransform::LocalToGlobal(const math::float4x4& local_transform)
{
	math::float4x4 global = local_transform;
	SetTransform(global);
}

void ComponentTransform::GlobalToLocal(const math::float4x4& global_transform)
{
	math::float4x4 global = math::float4x4::FromTRS(position, rotation, scale);
	math::float4x4 local = global_transform.Inverted() * global;
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

		if (ImGui::Button("Default"))
		{
			position = float3::zero;
			euler_rotation = float3::zero;
			rotation = rotation.FromEulerXYZ(math::DegToRad(euler_rotation.x),
				math::DegToRad(euler_rotation.y), math::DegToRad(euler_rotation.z));
			scale = float3::one;
			if (my_go->static_GO)
				App->scene->dirty = true;
		}

		// Show scaled position
		math::float3 scaled_position = position / App->editor->scale;

		if(ImGui::DragFloat3("Position", (float*)&scaled_position))
		{
			position = scaled_position * App->editor->scale;
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