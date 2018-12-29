#ifndef __COMPOMENTMATERIAL_H__
#define __COMPOMENTMATERIAL_H__

#include "Component.h"

class GameObject;
class Texture;

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

	JSON_value* Save(JSON_value* component) const override;
	void Load(JSON_value* component) override;

public:
	unsigned shader = 0;
	// Diffuse
	Texture* texture = nullptr;
	math::float4 color = math::float4::zero;
	float k_diffuse = 0.5f;
	// Specular
	Texture* specular_map = nullptr;
	math::float3 specular_color = math::float3::zero;
	float shininess = 64.0f;
	float k_specular = 1.0f;
	// Ambient
	Texture* occlusion_map = nullptr;
	float k_ambient = 0.5f;
	// Emissive
	Texture* emissive_map = nullptr;
	math::float3 emissive_color = math::float3::zero;
};

#endif /*__COMPOMENTMATERIAL_H__*/