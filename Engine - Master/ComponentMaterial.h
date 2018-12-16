#ifndef __COMPOMENTMATERIAL_H__
#define __COMPOMENTMATERIAL_H__

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
	void RenderMaterial();
	void Delete() override;

	void Save(JSON_value* component) const override;
	void Load(JSON_value* component) override;

public:
	unsigned shader = 0;
	Texture* texture = nullptr;
	math::float4 color = math::float4::zero;
	float shininess = 64.0f;
	float k_specular = 1.0f;
	float k_diffuse = 0.5f;
	float k_ambient = 0.5f;

};

#endif /*__COMPOMENTMATERIAL_H__*/