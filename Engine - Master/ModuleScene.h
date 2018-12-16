#ifndef __MODULESCENE_H__
#define __MODULESCENE_H__

#include "Module.h"
#include "GameObject.h"
//#include <vector>

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

	bool Save(JSON_file* document);
	void NewScene();
	bool SaveScene(const char* scene_name);
	bool LoadScene(const char* scene_name);

public:
	GameObject* root = nullptr;
	bool show_root = false;
};

#endif /*__MODULESCENE_H__*/