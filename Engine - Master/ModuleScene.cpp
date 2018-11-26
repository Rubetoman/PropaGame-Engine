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
}

GameObject* ModuleScene::CreateGameObject(const char* name)
{
	GameObject* go;
	// Check the pointer isn't null
	if (name != nullptr)
		go = new GameObject(name, root);
	else
		go = new GameObject(GO_DEFAULT_NAME, root);

	return go;
}

GameObject* ModuleScene::CreateGameObject(const char* name, GameObject* parent)
{
	GameObject* go;
	// Check the pointers aren't null
	if (parent != nullptr)
	{
		if (name != nullptr)
			go = new GameObject(name, parent);
		else
			go = new GameObject(GO_DEFAULT_NAME, parent);
	}
	else
	{
		LOG("Warning: parent == nullptr, creating GameObject without parent. \n ")
		go = CreateGameObject(name);
	}

	return go;
}

GameObject* ModuleScene::CreateGameObject(const char* name, math::float4x4& transform)
{
	GameObject* go;
	// Check the pointers aren't null
	if (name != nullptr)
		go = new GameObject(name, transform, root);
	else
		go = new GameObject(GO_DEFAULT_NAME, transform, root);
	
	return go;
}

GameObject* ModuleScene::CreateGameObject(const char* name, math::float4x4& transform, GameObject* parent)
{
	GameObject* go;
	// Check the pointers aren't null
	if (parent != nullptr)
	{
		if (name != nullptr)
			go = new GameObject(name, transform, parent);
		else
			go = new GameObject(GO_DEFAULT_NAME, transform, parent);
	}
	else
	{
		LOG("Warning: parent == nullptr, creating GameObject without parent. \n ")
		go = CreateGameObject(name, transform);
	}
	return go;
}