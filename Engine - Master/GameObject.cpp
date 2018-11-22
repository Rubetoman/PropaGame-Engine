#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void Update()
{
}

Component* GameObject::CreateComponent(component_type type)
{
	Component* component = nullptr;

	switch (type)
	{
	case component_type::Transform:
		//component = new ComponentTransform(this);
		//transform = (ComponentTransform*)component;
		break;
	case component_type::Mesh:
		//component = new ComponentMesh(this);
		break;
	case component_type::Material:
		//component = new ComponentMaterial(this);
		break;
	default:
		break;
	}
	components.push_back(component);
	return component;
}