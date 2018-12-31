#ifndef __COMPOMENTCAMERA_H__
#define __COMPOMENTCAMERA_H__

#include "Component.h"
#include "Geometry/AABB.h"

class GameObject;
class Camera;
class WindowCamera;

class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* go);
	ComponentCamera(const ComponentCamera& comp);
	~ComponentCamera();

	void Update() override;

	Component* Duplicate() override;
	void Delete() override;
	bool DrawOnInspector();

	void CreateFrameBuffer();

	void			TranslateCamera(math::float3 direction);
	void			Rotate(float dx, float dy);
	void			Orbit(float dx, float dy);
	void			LookAt(math::float3& target);

	bool ContainsAABB(const math::AABB& boundingBox) const;

	// Frustum
	const void		InitFrustum();
	void			SetFrustum(unsigned& w, unsigned& h);

	//JSON
	JSON_value* Save(JSON_value* component) const;
	void Load(JSON_value* component);
public:

	// Texture rendered to
	unsigned		fbo = 0u;
	unsigned		rbo = 0u;
	unsigned		renderedTexture = 0u;

	float			speed = 10.0f;
	float			rotation_speed = 65.0f;

	// Camera frustum
	math::Frustum frustum;
	bool frustum_culling = true;

	// Camera Window
	WindowCamera* window = nullptr;
};

#endif /*__COMPOMENTCAMERA_H__*/