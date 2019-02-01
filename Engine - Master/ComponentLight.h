#ifndef __COMPOMENTLIGHT_H__
#define __COMPOMENTLIGHT_H__

#include "Component.h"

enum class light_type { Point = 0, Directional, Spot, Light_Count};

class ComponentLight : public Component
{
public:
	ComponentLight(GameObject* go);
	ComponentLight(const ComponentLight& comp);
	~ComponentLight();

	Component* Duplicate() override;

	void Update() override;
	bool DrawOnInspector() override;

	float GetAttenuationDistance() const;

	JSON_value* Save(JSON_value* component) const override;
	void Load(JSON_value* component) override;

public:
	light_type type = light_type::Point;


	math::float3 color = math::float3::one;
	math::float3 attenuation = math::float3(0.1f, 0.1f, 0.1f);	// Attenuation(Constant, Linear, Quadric)

	// Only for directional & spot
	math::float3 direction = math::float3::one;

	// Only for spot
	float inner_cutoff = 20.f;
	float outer_cutoff = 25.f;
};

#endif /*__COMPOMENTCOMPONENT_H__*/