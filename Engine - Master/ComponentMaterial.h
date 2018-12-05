#ifndef __COMPOMENTCOMPONENT_H__
#define __COMPOMENTCOMPONENT_H__

#include "Component.h"
#include "GameObject.h"
#include "Texture.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* go);
	ComponentMaterial(const ComponentMaterial& comp);
	~ComponentMaterial();

	Component* Duplicate() override;
	bool DrawOnInspector() override;
	void Delete() override;

	Texture* texture = nullptr;
	math::float4 color = math::float4::one;

};

#endif /*__COMPOMENTCOMPONENT_H__*/