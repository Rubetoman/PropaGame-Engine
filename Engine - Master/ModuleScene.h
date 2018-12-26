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

	void Draw(const math::float4x4& view, const math::float4x4& proj);

	GameObject* CreateGameObject(const char* name);
	GameObject* CreateGameObject(const char* name, GameObject* parent);
	GameObject* CreateGameObject(const char* name, math::float4x4& transform);
	GameObject* CreateGameObject(const char* name, math::float4x4& transform, GameObject* parent);
	GameObject* CreateSphere(unsigned size, unsigned slices, unsigned stacks, GameObject* parent);

	void DeleteGameObject(GameObject* go);
	GameObject* DuplicateGameObject(const GameObject* go);
	void Unchild(GameObject* go);

	unsigned GetSceneGONumber(GameObject& go) const;

	bool Save(JSON_file* document);
	bool InitScene();
	void NewScene();
	bool SaveScene(const char* scene_name);
	bool LoadScene(const char* scene_name);
	bool DeleteScene(const char* scene_name);

public:
	std::vector<GameObject*> scene_gos;
	GameObject* root = nullptr;
	bool show_scene_gos = false;
	std::string name = "";
};

#endif /*__MODULESCENE_H__*/