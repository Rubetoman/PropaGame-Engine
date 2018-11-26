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
	bool CleanUp();

	void Draw();

	GameObject* CreateGameObject(const char* name);
	GameObject* CreateGameObject(const char* name, GameObject* parent);
	GameObject* CreateGameObject(const char* name, math::float4x4& transform);
	GameObject* CreateGameObject(const char* name, math::float4x4& transform, GameObject* parent);

	GameObject* root = nullptr;
	std::vector<GameObject*> game_objects;
};

#endif /*__MODULESCENE_H__*/