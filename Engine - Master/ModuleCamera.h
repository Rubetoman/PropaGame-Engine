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
	void			InitProyection();
	void			UpdateProyection();

	float4x4 model_view;
	float4x4 view_matrix;
	float4x4 result_matrix;
	float3 eye = { 0, 0, 5 };
	float3 target = { 0, 0, 0 };
	float3 up = { 0, 0, 1 };
	float4x4 proj;

private:
	unsigned vbo = 0;

};

#endif /* __ModuleCamera_h__ */

