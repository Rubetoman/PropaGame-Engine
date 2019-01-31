#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "JSON.h"

#include <string>

class GameObject;

enum class component_type {Transform, Mesh, Material, Light, Camera, Editor_Camera};

class Component
{
public:
	Component(GameObject* go, component_type type);
	Component(const Component& comp);
	virtual ~Component();

	virtual Component* Duplicate() = 0;
	virtual bool DrawOnInspector();
	virtual void Init() {}
	virtual void Update();
	virtual void Enable();
	virtual void Disable();
	virtual void Delete();

	unsigned GetComponentNumber() const;

	virtual JSON_value* Save(JSON_value* component) const;
	virtual void Load(JSON_value* component);

public:
	std::string uuid = "";
	std::string my_go_uid = "";
	component_type type;
	bool active = true;
	GameObject* my_go = nullptr;
};

#endif /*__COMPONENT_H__*/