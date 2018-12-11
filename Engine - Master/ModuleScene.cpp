#include "ModuleScene.h"

#include "ModuleModelLoader.h"
#include <vector>

ModuleScene::ModuleScene()
{
	root = new GameObject("World");
	//game_objects.push_back(root);
}


ModuleScene::~ModuleScene()
{
}

update_status ModuleScene::Update()
{
	root->Update();
	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	if (root != nullptr)
	{
		root->CleanUp();
	}
	return true;
}

void ModuleScene::Draw()
{
	if(root != nullptr)
		root->Draw();
}

GameObject* ModuleScene::CreateGameObject(const char* name)
{
	GameObject* go;

	// Create a root in case it was deleted
	if(root == nullptr)
		root = new GameObject("World");

	// Check the pointer isn't null
	if (name != nullptr)
	{
		go = new GameObject(name, root);
	}	
	else	// Give default name
	{
		go = new GameObject(GO_DEFAULT_NAME, root);
	}	
	return go;
}

GameObject* ModuleScene::CreateGameObject(const char* name, GameObject* parent)
{
	GameObject* go;

	// Create a root in case it was deleted
	if (root == nullptr)
		root = new GameObject("World");

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

	// Create a root in case it was deleted
	if (root == nullptr)
		root = new GameObject("World");

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

	// Create a root in case it was deleted
	if (root == nullptr)
		root = new GameObject("World");

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

GameObject* ModuleScene::CreateSphere(unsigned size, unsigned slices, unsigned stacks, GameObject* parent)
{
	GameObject* go;

	// Create a root in case it was deleted
	if (root == nullptr)
		root = new GameObject("World");

	go = App->model_loader->CreateSphere("sphere", math::float3(0.0f, 0.0f, 0.0f), Quat::identity, math::float3(size, size, size), slices, stacks, float4(0.f, 0.0f, 0.5f, 1.0f));
	
	// Set parent
	go->Unchild();
	go->parent = parent;
	parent->children.push_back(go);

	return go;
}

void ModuleScene::DeleteGameObject(GameObject* go)
{
	if (go == nullptr)
	{
		LOG("Warning: GameObject == nullptr.");
		return;
	}
	if (go == root)
		root = nullptr;

	go->DeleteGameObject();
}

GameObject* ModuleScene::DuplicateGameObject(const GameObject* go)
{
	GameObject* new_go = new GameObject(*go);
	new_go->parent = go->parent;
	new_go->parent->children.push_back(new_go);
	return new_go;
}

void ModuleScene::Unchild(GameObject* go)
{
	if (go == nullptr)
	{
		LOG("Warning: GameObject == nullptr.");
		return;
	}
	go->SetParent(root);
}