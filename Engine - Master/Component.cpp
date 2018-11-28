#include "Component.h"
#include "GameObject.h"


Component::Component(GameObject* go, component_type type) : my_go(go), type(type)
{
}


Component::~Component()
{
}

void Component::Update()
{

}

void Component::DrawOnInspector()
{
	ImGui::Checkbox("Active", &active);
}

void Component::CleanUp()
{

}