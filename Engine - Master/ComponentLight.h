#ifndef __COMPOMENTLIGHT_H__
#define __COMPOMENTLIGHT_H__

#include "Component.h"
class ComponentLight : public Component
{
public:
	ComponentLight(GameObject* go);
	ComponentLight(const ComponentLight& comp);
	~ComponentLight();

	Component* Duplicate() override;
	void Delete() override;
	bool DrawOnInspector() override;

	JSON_value* Save(JSON_value* component) const override;
	void Load(JSON_value* component) override;
public:
	float intensity = 0.4f;
};

#endif /*__COMPOMENTCOMPONENT_H__*/