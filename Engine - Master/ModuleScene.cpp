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
	Draw();
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

void ModuleScene::Draw()
{
	// Warning: once child and parent are included to GO
	//root->Draw();
	
	for (auto &go : game_objects)
	{
		go->Draw();
	}
}

GameObject* ModuleScene::CreateGameObject(const char* name)
{
	GameObject* go = new GameObject(name);
	game_objects.push_back(go);
	return go;
}