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
	root->Draw();
	
	/*for (auto &go : game_objects)
	{
		go->Draw();
	}*/
}

GameObject* ModuleScene::CreateGameObject(const char* name)
{
	GameObject* go = new GameObject(name, root);
	return go;
}

GameObject* ModuleScene::CreateGameObject(const char* name, math::float4x4& transform)
{
	GameObject* go = new GameObject(name, transform, root);
	return go;
}