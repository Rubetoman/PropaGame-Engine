#ifndef __ModuleCamera_h__
#define __ModuleCamera_h__

#include "Module.h"
#include "ModuleWindow.h"
#include "Application.h"
#include "ModuleInput.h"
#include "GL/glew.h"
#include "SDL.h"
#include "MathGeoLib.h"
#include "SDL/include/SDL.h"

class ModuleCamera : public Module
{
	enum CameraRotation {
		PositivePitch,
		NegativePitch,
		PositiveYaw,
		NegativeYaw
	};

public:
	ModuleCamera();
	~ModuleCamera();

	bool            Init();
	update_status	PreUpdate();
	update_status   Update();
	bool            CleanUp();

	math::float4x4	ProjectionMatrix();
	// Frustum
	void			InitFrustum();
	void			SetHorizontalFOV(float& degrees);
	void			SetVerticalFOV(float& degrees);
	void			UpdateScreenSize();

	math::float4x4	LookAt(math::float3& target, math::float3& eye);
	void			TranslateCameraInput();
	void			RotateCameraInput();
	void			CameraSpeedInput(float modifier);
	void			RotateCamera();
	void			MouseUpdate(math::float2& mouse_new_pos);

	math::float4x4 model_view;
	math::float4x4 view_matrix;
	math::float4x4 result_matrix;
	math::float4x4 proj;
	math::Frustum frustum;

	// Camera rotation
	float pitch;
	float yaw;

	// Camera vectors
	math::float3& cam_position = math::float3(0, 0, 0);	// Position in world of the camera
	math::float3& cam_target = math::float3(0, 0, 0);	// Where the camera is looking at
	math::float3& cam_up = math::float3(0, 0, 0);		// Up vector of the camera
	float cam_speed = 1.0f;
	float cam_rot_speed = 65.0f;

	// Mouse 
	bool firstMouse = true;
	float lastX = SCREEN_WIDTH / 2;
	float lastY = SCREEN_HEIGHT / 2;

private:
	unsigned vbo = 0;

};

#endif /* __ModuleCamera_h__ */

