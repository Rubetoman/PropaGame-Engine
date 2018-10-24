#ifndef __ModuleRenderExercise_h__
#define __ModuleRenderExercise_h__

#include "Module.h"

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

};

#endif /* __ModuleRenderExercise_h__ */