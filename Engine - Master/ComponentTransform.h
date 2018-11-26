#ifndef __COMPOMENTTRANSFORM_H__
#define __COMPOMENTTRANSFORM_H__

#include "Component.h"
#include "MathGeoLib.h"

class ComponentTransform : public Component
{ 
public:
	ComponentTransform(GameObject* go);
	~ComponentTransform();

	void SetTransform(const math::float4x4& transform);

	void DrawOnInspector() override;

	math::float3 position = math::float3().zero;
	math::float3 euler_rotation = math::float3().zero;
	math::Quat rotation = math::Quat().identity;
	math::float3 scale = math::float3().zero;
};

#endif /*__COMPOMENTTRANSFORM_H__*/