#include "GameObject.h"
#include "Globals.h"

#include "Component.h"
#include "ComponentTransform.h"

GameObject::GameObject(const char * name) : name(name)
{
	CreateComponent(component_type::Transform);
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
}

void GameObject::CleanUp()
{
	for (auto &component : components)
	{
		RELEASE(component);
	}

	for (auto &child : children)
	{
		RELEASE(child);
	}

	transform = nullptr;
	parent = nullptr;
}

Component* GameObject::CreateComponent(component_type type)
{
	Component* component = nullptr;

	switch (type)
	{
	case component_type::Transform:
		component = new ComponentTransform();
		transform = (ComponentTransform*)component;
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