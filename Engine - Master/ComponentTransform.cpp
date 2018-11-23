#include "ComponentTransform.h"


ComponentTransform::ComponentTransform() : Component()
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
		ImGui::DragFloat3("Rotation", (float*)&rotation, 0.1f);
		ImGui::DragFloat3("Scale", (float*)&scale, 0.1f);
	}
}

		

			