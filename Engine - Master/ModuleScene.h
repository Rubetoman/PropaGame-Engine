#ifndef __MODULESCENE_H__
#define __MODULESCENE_H__

#include "Module.h"
#include "GameObject.h"
#include <vector>

class ModuleScene : public Module
{
public:
	ModuleScene();
	~ModuleScene();

	update_status Update();

	GameObject* CreateGameObject();

	std::vector<GameObject*> game_objects;
};

#endif /*__MODULESCENE_H__*/