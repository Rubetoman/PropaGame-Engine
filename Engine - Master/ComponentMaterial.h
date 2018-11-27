#ifndef __COMPOMENTCOMPONENT_H__
#define __COMPOMENTCOMPONENT_H__

#include "Component.h"
#include "Texture.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* go);
	~ComponentMaterial();

	//void DrawOnInspector() override;

	Texture* texture;
};

#endif /*__COMPOMENTCOMPONENT_H__*/