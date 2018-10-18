#ifndef __ModuleRenderExercise_h__
#define __ModuleRenderExercise_h__

#include "Module.h"
#include "MathGeoLib.h"

class ModuleRenderExercise : public Module
{
public:
    ModuleRenderExercise();
    ~ModuleRenderExercise();

	bool            Init    ();
	update_status   Update  ();
	bool            CleanUp ();
	float4x4 model_view;
	float4x4 view_matrix;
	float4x4 result_matrix;
	float3 eye = { 0, 0, 5 };
	float3 target = { 0, 0, 0 };
	float3 up = { 0, 0, 1 };


private:
    unsigned vbo        = 0;

};

#endif /* __ModuleRenderExercise_h__ */
