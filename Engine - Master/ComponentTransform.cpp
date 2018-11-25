#include "ComponentTransform.h"


ComponentTransform::ComponentTransform(GameObject* go) : Component(go, component_type::Transform)
{
}

ComponentTransform::~ComponentTransform()
{
}
void ComponentTransform::DrawOnInspector()
{
	Component::DrawOnInspector();
	if (ImGui::CollapsingHeader("Transform Component"))
	{
		ImGui::DragFloat3("Position", (float*)&position, 0.1f);
		ImGui::DragFloat3("Rotation", (float*)&euler_rotation, 0.1f);
		ImGui::DragFloat3("Scale", (float*)&scale, 0.1f);
	}
}

		

			