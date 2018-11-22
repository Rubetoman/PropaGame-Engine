#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Component.h"
#include <string>
#include <vector>

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();
	Component* CreateComponent(component_type type);

	bool active = true;
	std::string name;
	

	// Components
	// TransformComponent* transform = nullptr;
	std::vector<Component*> components;
};

#endif /*__GAMEOBJECT_H__*/