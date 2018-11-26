#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"

class GameObject;

enum class component_type {Transform, Mesh, Material};

class Component
{
public:
	Component(GameObject* go, component_type type);
	virtual ~Component();

	virtual void DrawOnInspector();
	virtual void Enable() {active = true;};
	virtual void Update();
	virtual void Disable() { active = false; };

	virtual void CleanUp();

public:
	component_type type;
	bool active = true;
	GameObject* my_go = nullptr;
};

#endif /*__COMPONENT_H__*/