#ifndef __COMPOMENTCAMERA_H__
#define __COMPOMENTCAMERA_H__

#include "Component.h"
#include "Geometry/AABB.h"

class GameObject;
class Camera;

class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* go);
	ComponentCamera(const ComponentCamera& comp);
	~ComponentCamera();

	Component* Duplicate() override;
	bool DrawOnInspector();

	unsigned GetCameraNumber() const;
	void CreateFrameBuffer();

public:
	Camera* camera = nullptr;
	math::AABB boundingBox;

	// Texture rendered to
	unsigned		fbo = 0u;
	unsigned		rbo = 0u;
	unsigned		renderedTexture = 0u;
};

#endif /*__COMPOMENTCAMERA_H__*/