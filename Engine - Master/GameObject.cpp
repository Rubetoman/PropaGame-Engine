#include "GameObject.h"
#include "Globals.h"

#include "ModuleCamera.h"
#include "ModuleRender.h"

#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

GameObject::GameObject(const char * name) : name(name)
{
	CreateComponent(component_type::Transform);
}

GameObject::GameObject(const char* name, GameObject* parent) : name(name), parent(parent)
{
	CreateComponent(component_type::Transform);
	parent->children.push_back(this);
}

GameObject::GameObject(const char* name, const math::float4x4& new_transform) : name(name)
{
	transform = (ComponentTransform*)CreateComponent(component_type::Transform);
	transform->SetTransform(new_transform);
}

GameObject::GameObject(const char* name, const math::float4x4& new_transform, GameObject* parent) 
	: name(name), parent(parent)
{
	transform = (ComponentTransform*)CreateComponent(component_type::Transform);
	transform->SetTransform(new_transform);
	parent->children.push_back(this);
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

	for (const auto &child : children)
	{
		child->Draw();
	}
	unsigned program = App->renderer->programText;
	//Draw meshes
	glUseProgram(program);

	math::float4x4 proj = App->camera->mainCamera->ProjectionMatrix();
	math::float4x4 view = App->camera->mainCamera->LookAt(App->camera->mainCamera->position + App->camera->mainCamera->front);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, (const float*)&GetGlobalTransform()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (const float*)&view);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, (const float*)&proj);
	
	if (mesh != nullptr && mesh->active)
	{
		((ComponentMesh*)mesh)->RenderMesh(App->renderer->programText, material->texture, view, proj);
	}
	glUseProgram(0);
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
		if (mesh == nullptr)
		{
			component = new ComponentMesh(this);
			mesh = (ComponentMesh*)component;
		}
		else
		{
			LOG("Warning: %s already has a Mesh Component attached.", name);
		}
		break;
	case component_type::Material:
		if (material == nullptr)
		{
			component = new ComponentMaterial(this);
			material = (ComponentMaterial*)component;
		}
		else
		{
			LOG("Warning: %s already has a Material Component attached.", name);
		}
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

int GameObject::GetChildNumber() const
{
	if (parent == nullptr)
	{
		LOG("Warning: %s doesn't have a parent", name);
		return -1;
	}
	auto pos = std::find(parent->children.begin(), parent->children.end(), this) - parent->children.begin();
	if (pos >= parent->children.size())
	{
		LOG("Warning: GameObject not found as a child of %s.", parent->name);
		return -1;
	}
	return pos;
}

void GameObject::DeleteGameObject()
{
	Unchild();
	CleanUp();
	delete this;
}

void GameObject::Unchild()
{
	if (parent == nullptr)
	{
		LOG("Warning: %s doesn't have a parent.", name);
		return;
	}
	parent->children.erase(parent->children.begin() + GetChildNumber());
}

math::float4x4 GameObject::GetLocalTransform() const
{
	if (transform == nullptr) 
		return float4x4::identity;

	return float4x4::FromTRS(transform->position, transform->rotation, transform->scale);
}

math::float4x4 GameObject::GetGlobalTransform() const
{
	if (parent != nullptr)
		return parent->GetGlobalTransform() * GetLocalTransform();

	return GetLocalTransform();
}