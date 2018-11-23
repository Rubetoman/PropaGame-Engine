#ifndef __COMPOMENTTRANSFORM_H__
#define __COMPOMENTTRANSFORM_H__

#include "Component.h"
#include "MathGeoLib.h"

class ComponentTransform : public Component
{ 
public:
	ComponentTransform();
	~ComponentTransform();

	float3 position = float3().zero;
	float3 rotation = float3().zero;
	float3 scale = float3().zero;
};

#endif /*__COMPOMENTTRANSFORM_H__*/