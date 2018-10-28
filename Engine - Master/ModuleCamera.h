#ifndef __ModuleCamera_h__
#define __ModuleCamera_h__

#include "Module.h"
#include "MathGeoLib.h"
#include "SDL/include/SDL.h"

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool            Init();
	update_status	PreUpdate();
	update_status   Update();
	bool            CleanUp();
	float4x4		ProjectionMatrix();
	void			InitProyection();
	float4x4		LookAt(math::float3& target, math::float3& eye, math::float3& up);
	void			TranslateCameraInput();
	void			ChangeCameraSpeed(float modifier);

	float4x4 model_view;
	float4x4 view_matrix;
	float4x4 result_matrix;
	float4x4 proj;
	Frustum frustum;

	// Camera
	float3& eye = float3(0, 0, 0);
	float3& target = float3(0, 0, 0);
	float3& up = float3(0, 0, 0);
	float3 forward_v;
	float3 side_v;
	float3 up_v;
	float cam_speed = 0.01f;

private:
	unsigned vbo = 0;

};

#endif /* __ModuleCamera_h__ */

