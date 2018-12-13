#ifndef __MODULESCENE_H__
#define __MODULESCENE_H__

#include "Module.h"
#include "GameObject.h"
#include <vector>
#include "pcg_random.hpp"

class ModuleScene : public Module
{
public:
	ModuleScene();
	~ModuleScene();

	bool Init() override;
	update_status Update() override;
	bool CleanUp() override;

	void Draw();

	GameObject* CreateGameObject(const char* name);
	GameObject* CreateGameObject(const char* name, GameObject* parent);
	GameObject* CreateGameObject(const char* name, math::float4x4& transform);
	GameObject* CreateGameObject(const char* name, math::float4x4& transform, GameObject* parent);
	GameObject* CreateSphere(unsigned size, unsigned slices, unsigned stacks, GameObject* parent);

	void DeleteGameObject(GameObject* go);
	GameObject* DuplicateGameObject(const GameObject* go);
	void Unchild(GameObject* go);

	inline unsigned GetNewUID();

public:
	pcg32 uuid_rng;
	std::vector<GameObject*> lights;	// List of all the lights on the scene

	GameObject* root = nullptr;
	bool show_root = false;
};

#endif /*__MODULESCENE_H__*/