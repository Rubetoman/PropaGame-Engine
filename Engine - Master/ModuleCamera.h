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
	math::float4x4	LookAt(math::float3& target, math::float3& eye, math::float3& up);

	float4x4 model_view;
	float4x4 view_matrix;
	float4x4 result_matrix;
	float3& eye = float3(0, 0, 0);
	float3& target = float3(0, 0, 0);
	float3& up = float3(0, 0, 0);
	float4x4 proj;
	Frustum frustum;

private:
	unsigned vbo = 0;

};

#endif /* __ModuleCamera_h__ */

