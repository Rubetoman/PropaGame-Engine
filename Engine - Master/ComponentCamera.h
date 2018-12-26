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

	Component* Duplicate() override;
	void Delete() override;
	bool DrawOnInspector();

	void CreateFrameBuffer();

	void			TranslateCamera(math::float3 direction);
	void			RotateCamera();
	math::float4x4	LookAt(math::float3& target);

	// Frustum
	const void		InitFrustum();
	void			SetFrustum(unsigned& w, unsigned& h);
	const void		UpdatePitchYaw();

	//JSON
	JSON_value* Save(JSON_value* component) const;
	void Load(JSON_value* component);
public:
	math::AABB boundingBox;

	// Texture rendered to
	unsigned		fbo = 0u;
	unsigned		rbo = 0u;
	unsigned		renderedTexture = 0u;

	// Camera vectors
	math::float3&	front = math::float3(0, 0, -1);		// Where the camera is looking at
	math::float3&	side = math::float3(0, 0, 0);
	math::float3&	up = math::float3(0, 1, 0);			// Up vector of the camera

	float			speed = 10.0f;
	float			rotation_speed = 65.0f;

	// Camera frustum
	math::float4x4 proj;
	math::Frustum frustum;

	math::float4x4 view_matrix;

	// Camera rotation
	float pitch = 0.0f;
	float yaw = -90.0f;

	// Camera Window
	WindowCamera* window = nullptr;
};

#endif /*__COMPOMENTCAMERA_H__*/