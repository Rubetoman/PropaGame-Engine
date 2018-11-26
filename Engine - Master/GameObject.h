#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "MathGeoLib.h"
#include <string>
#include <vector>

#define GO_NAME_SIZE 24

class Component;
class ComponentTransform;
enum class component_type;

class GameObject
{
public:
	GameObject(const char* name);
	GameObject(const char* name, GameObject* parent);
	GameObject(const char* name, const math::float4x4& new_transform);
	GameObject(const char* name, const math::float4x4& new_transform, GameObject* parent);
	~GameObject();

	void Update();
	void CleanUp();

	void Draw();

	Component* CreateComponent(component_type type);

	std::vector<Component*> GameObject::GetComponents(component_type type) const;

	bool active = true;
	std::string name = "GameObject";
	
	// Hierarchy
	GameObject* parent = nullptr;
	std::vector<GameObject*> children;

	// Components
	ComponentTransform* transform = nullptr;
	std::vector<Component*> components;
};

#endif /*__GAMEOBJECT_H__*/