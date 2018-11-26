#include "ComponentTransform.h"


ComponentTransform::ComponentTransform(GameObject* go) : Component(go, component_type::Transform)
{
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::SetTransform(const math::float4x4& transform)
{
	math::float3 new_position;
	math::float3 new_scale;
	math::Quat new_rotation;
	transform.Decompose(new_scale, new_rotation, new_position);

	position = { new_position.x, new_position.y, new_position.z };
	scale = { new_scale.x, new_scale.y, new_scale.z };
	rotation = new_rotation;
	euler_rotation = rotation.ToEulerXYZ();
	euler_rotation.x = math::RadToDeg(euler_rotation.x);
	euler_rotation.y = math::RadToDeg(euler_rotation.y);
	euler_rotation.z = math::RadToDeg(euler_rotation.z);
}

void ComponentTransform::DrawOnInspector()
{
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Transform Component"))
	{
		ImGui::DragFloat3("Position", (float*)&position, 0.1f);
		ImGui::DragFloat3("Rotation", (float*)&euler_rotation, 0.1f);
		ImGui::DragFloat3("Scale", (float*)&scale, 0.1f);
	}
}

		

			