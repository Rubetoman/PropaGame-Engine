#ifndef __ModuleRenderExercise_h__
#define __ModuleRenderExercise_h__

#include "Module.h"
#include "ModuleWindow.h"
#include "Application.h"
#include "ModuleShader.h"
#include "MathGeoLib.h"
#include "GL/glew.h"
#include "SDL.h"

class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();

	bool            Init();
	update_status   Update();
	bool            CleanUp();

private:
	unsigned vbo = 0;
	GLuint program = GL_FALSE;

};

#endif /* __ModuleRenderExercise_h__ */