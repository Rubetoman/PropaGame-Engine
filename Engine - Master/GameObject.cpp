#include "GameObject.h"
#include "Globals.h"

#include "ModuleCamera.h"
#include "ModuleRender.h"

#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"

GameObject::GameObject(const char * name) : name(name)
{
	CreateComponent(component_type::Transform);
}

GameObject::GameObject(const char* name, const aiMatrix4x4& transform) : name(name)
{
	this->transform = (ComponentTransform*)CreateComponent(component_type::Transform);
	
	aiVector3D translation;
	aiVector3D scaling;
	aiQuaternion airotation;
	transform.Decompose(scaling, airotation, translation);

	this->transform->position = { translation.x, translation.y, translation.z };
	this->transform->scale = { scaling.x, scaling.y, scaling.z };
	this->transform->rotation = Quat(airotation.x, airotation.y, airotation.z, airotation.w);
	this->transform->euler_rotation = this->transform->rotation.ToEulerXYZ();
	this->transform->euler_rotation.x = math::RadToDeg(this->transform->euler_rotation.x);
	this->transform->euler_rotation.y = math::RadToDeg(this->transform->euler_rotation.y);
	this->transform->euler_rotation.z = math::RadToDeg(this->transform->euler_rotation.z);
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
}

void GameObject::CleanUp()
{
	for (auto &component : components)
	{
		component->CleanUp();
		RELEASE(component);
	}

	for (auto &child : children)
	{
		child->CleanUp();
		RELEASE(child);
	}

	transform = nullptr;
	parent = nullptr;
}

void GameObject::Draw()
{
	if (!active) return;
	if (transform == nullptr) return;

	//Draw meshes
	math::float4x4 proj = App->camera->mainCamera->ProjectionMatrix();
	math::float4x4 view = App->camera->mainCamera->LookAt(App->camera->mainCamera->position + App->camera->mainCamera->front);
	
	std::vector<Component*> meshes = GetComponents(component_type::Mesh);
	for (auto &mesh : meshes)
	{
		if (mesh->active)
		{
			((ComponentMesh*)mesh)->RenderMesh(App->renderer->programText, view, proj);
		}
	}
}

Component* GameObject::CreateComponent(component_type type)
{
	Component* component = nullptr;

	switch (type)
	{
	case component_type::Transform:
		component = new ComponentTransform(this);
		transform = (ComponentTransform*)component;
		break;
	case component_type::Mesh:
		component = new ComponentMesh(this);
		break;
	case component_type::Material:
		//component = new ComponentMaterial(this);
		break;
	default:
		break;
	}
	components.push_back(component);
	return component;
}

std::vector<Component*> GameObject::GetComponents(component_type type) const
{
	std::vector<Component*> comp;
	for (auto &component : components)
	{
		if (component->type == type)
		{
			comp.push_back(component);
		}
	}
	return comp;
}