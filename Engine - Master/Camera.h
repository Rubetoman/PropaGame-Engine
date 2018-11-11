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
	
	math::float3&	position = math::float3(0, 0, 0);		// Position in world of the camera
	
	// Camera vectors
	math::float3&	front = math::float3(0, 0, 0);		// Where the camera is looking at
	math::float3&	side = math::float3(0, 0, 0);
	math::float3&	up = math::float3(0, 0, 0);			// Up vector of the camera

	float			speed = 1.0f;
	float			rotation_speed = 65.0f;

	// Camera transformation matrix
	//math::float4x4 model_view;
	//math::float4x4 view_matrix;
	//math::float4x4 result_matrix;

	// Camera frustum
	math::float4x4 proj;
	math::Frustum frustum;

	// Camera rotation
	float pitch = 0;
	float yaw = -90;

	void			TranslateCamera(math::float3 direction);
	void			RotateCamera();
	math::float4x4	LookAt(math::float3& target);
	
	// Frustum
	const void		InitFrustum();
	math::float4x4	ProjectionMatrix();
	void			SetHorizontalFOV(float& degrees);
	void			SetVerticalFOV(float& degrees);
	const void		UpdatePitchYaw();
};

#endif /* __CAMERA_H__ */
