#include "ModuleScene.h"

#include "ModuleModelLoader.h"
#include "ModuleResources.h"
#include "ModuleCamera.h"

#include "ComponentTransform.h"
#include "ComponentLight.h"

ModuleScene::ModuleScene()
{
}


ModuleScene::~ModuleScene()
{
	DeleteGameObject(root);
	delete root;
}

bool ModuleScene::Init()
{
	root = new GameObject("World");
	GameObject* default_light = CreateGameObject("Default Light", root);
	default_light->transform->position = math::float3(-2.0f, 0.0f, 6.0f);
	default_light->CreateComponent(component_type::Light);
	App->resources->lights.push_back(default_light);
	return true;
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

#pragma region scene management functions

bool ModuleScene::Save(JSON_file* document) 
{
	JSON_value* scene = document->createValue();
	scene->AddString("name", "scene");
	document->addValue("scene", scene);
	root->Save(scene);
	return true;
}

void ModuleScene::NewScene()
{
	root->DeleteGameObject();
	RELEASE(root);

	root = new GameObject("World");
}

bool ModuleScene::SaveScene(const char* scene_name)
{
	JSON_file* scene = App->json->openWriteFile(App->file->getFullPath(scene_name, SCENES_FOLDER, SCENES_EXTENSION).c_str());

	JSON_value* gameObjects = scene->createValue();
	gameObjects->convertToArray();

	root->Save(gameObjects);

	scene->addValue("Root", gameObjects);
	scene->Write();
	App->json->closeFile(scene);

	return true;
}

bool ModuleScene::LoadScene(const char* scene_name)
{
	NewScene();
	App->camera->mainCamera->LookAt(math::float3(0.0f, 0.0f, 0.0f));

	JSON_file* scene = App->json->openReadFile(App->file->getFullPath(scene_name, SCENES_FOLDER, SCENES_EXTENSION).c_str());

	JSON_value* go_root = scene->getValue("Root"); //It is an array of values

	std::map<std::string, GameObject*> gameobjects;
	for (int i = 0; i < go_root->getRapidJSONValue()->Size(); i++)
	{
		GameObject* go = new GameObject("");
		go->Load(go_root->getValueFromArray(i));
		gameobjects.insert(std::pair<std::string, GameObject*>(go->uuid, go));
		//App->camera->BBtoLook->Enclose(go->);
	}

	for (std::map<std::string, GameObject*>::iterator it_go = gameobjects.begin(); it_go != gameobjects.end(); it_go++)
	{
		if ((*it_go).second->parentUID == "") //If it has no parent, add it to the scene list
			root = (*it_go).second;
		else
		{
			GameObject* parent = gameobjects[(*it_go).second->parentUID];
			(*it_go).second->parent = parent;
			parent->children.push_back((*it_go).second);
			//parent->boundingBox.Enclose((*it_go).second->boundingBox);
		}
	}

	//App->camera->FitCamera(*App->camera->BBtoLook);
	App->json->closeFile(scene);
	return true;
}

#pragma endregion