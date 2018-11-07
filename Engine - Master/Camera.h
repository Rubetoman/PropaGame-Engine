#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "MathGeoLib.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "SDL.h"

class Camera
{
public:
	Camera();
	~Camera();
	
	// Camera vectors
	math::float3& cam_position = math::float3(0, 0, 0);	// Position in world of the camera
	math::float3& cam_front = math::float3(0, 0, 0);	// Where the camera is looking at
	math::float3& cam_up = math::float3(0, 0, 0);		// Up vector of the camera
	float cam_speed = 1.0f;
	float cam_rot_speed = 65.0f;

	// Camera transformation matrix
	math::float4x4 model_view;
	math::float4x4 view_matrix;
	math::float4x4 result_matrix;

	// Camera frustum
	math::float4x4 proj;
	math::Frustum frustum;

	// Camera rotation
	float pitch = 0;
	float yaw = -90;

	void			TranslateCamera(math::float3 direction);
	void			RotateCamera();
	math::float4x4	LookAt(math::float3& target, math::float3& eye);
	// Frustum
	void			InitFrustum();
	math::float4x4	ProjectionMatrix();
	void			SetHorizontalFOV(float& degrees);
	void			SetVerticalFOV(float& degrees);
	void			UpdatePitchYaw();
};

#endif /* __Camera_H__ */
