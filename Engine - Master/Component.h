#ifndef __COMPONENT_H__
#define __COMPONENT_H__

class GameObject;

enum class component_type {Transform, Mesh, Material};

class Component
{
public:
	Component();
	virtual ~Component();

	virtual void DrawOnInspector() {}
	virtual void Enable() {active = true;};
	virtual void Update();
	virtual void Disable() { active = false; };

public:
	component_type type;
	bool active = true;
	GameObject* my_go = nullptr;
};

#endif /*__COMPONENT_H__*/