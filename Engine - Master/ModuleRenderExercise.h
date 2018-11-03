#ifndef __MODULERENDEREXERCISE_h__
#define __MODULERENDEREXERCISE_h__

#include "Module.h"
#include "MathGeoLib.h"
#include "ModuleWindow.h"
#include "Application.h"
#include "ModuleShader.h"
#include "ModuleCamera.h"
#include "ModuleTextures.h"
#include "MathGeoLib.h"
#include "GL/glew.h"
#include "SDL.h"
#include <IL/il.h>
#include <IL/ilut.h>

class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();

	bool            Init();
	update_status   Update();
	bool            CleanUp();
	void			DrawCoordinates();
	void			DrawPlane();

	float4x4 model_view;
	float4x4 view_matrix;
	float4x4 result_matrix;
	float3 eye = { 0, 0, 5 };
	float3 target = { 0, 0, 0 };
	float3 up = { 0, 0, 1 };

	GLuint texture;
	image desatranques, sankara, pazos, logo;

	GLuint program = GL_FALSE;
	GLuint programText = GL_FALSE;
private:
	unsigned vbo = 0;

};

#endif /* __ModuleRenderExercise_h__ */