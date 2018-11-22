#ifndef __COMPONENT_H__
#define __COMPOMENT_H__

class GameObject;

enum class component_type {Transform, Mesh, Material};

class Component
{
public:
	Component();
	~Component();

	component_type type;
	bool active = true;
	GameObject* my_go = nullptr;
	
	virtual void Enable() {active = true;};
	virtual void Update();
	virtual void Disable() { active = false; };
};

#endif /*__COMPONENT_H__*/