#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <string>
#include <vector>

#define GO_NAME_SIZE 24

class Component;
class ComponentTransform;
enum class component_type;

class GameObject
{
public:
	GameObject(const char * name);
	~GameObject();

	void Update();
	void CleanUp();

	Component* CreateComponent(component_type type);

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