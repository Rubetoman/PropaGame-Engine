#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <string>
#include <vector>

class Component;
class ComponentTransform;
enum class component_type;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();
	void CleanUp();

	Component* CreateComponent(component_type type);

	bool active = true;
	std::string name;
	
	// Hierarchy
	GameObject* parent = nullptr;
	std::vector<GameObject*> children;

	// Components
	ComponentTransform* transform = nullptr;
	std::vector<Component*> components;
};

#endif /*__GAMEOBJECT_H__*/