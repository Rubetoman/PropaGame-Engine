#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "JSON.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include <string>

class GameObject;

enum class component_type {Transform, Mesh, Material, Light};

class Component
{
public:
	Component(GameObject* go, component_type type);
	Component(const Component& comp);
	virtual ~Component();

	virtual Component* Duplicate() = 0;
	virtual bool DrawOnInspector();
	virtual void Enable() {active = true;};
	virtual void Update();
	virtual void Disable() { active = false; };

	virtual void CleanUp();
	virtual void Delete();

	int GetComponentNumber() const;

	virtual void Save(JSON_value* component) const;
	virtual void Load(JSON_value* component);

public:
	std::string uuid = "";
	std::string my_go_uid = "";
	component_type type;
	bool active = true;
	GameObject* my_go = nullptr;
};

#endif /*__COMPONENT_H__*/