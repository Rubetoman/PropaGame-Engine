#include "ModuleScene.h"

#include <vector>

ModuleScene::ModuleScene()
{
	root = new GameObject("World");
	game_objects.push_back(root);
}


ModuleScene::~ModuleScene()
{
}

update_status ModuleScene::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		RELEASE((*it));
	}
	game_objects.clear();
	return true;
}

GameObject* ModuleScene::CreateGameObject(const char* name)
{
	GameObject* go = new GameObject(name);
	game_objects.push_back(go);
	return go;
}