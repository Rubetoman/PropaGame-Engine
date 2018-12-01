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
	for (auto &component : components)
	{
		RELEASE(component);
	}

	for (auto &child : children)
	{
		RELEASE(child);
	}

	transform = nullptr;
	mesh = nullptr;
	material = nullptr;
	parent = nullptr;
}

void GameObject::Update()
{
	for (std::list<GameObject*>::iterator it_child = children.begin(); it_child != children.end();)
	{
		(*it_child)->Update();

		if ((*it_child)->flags & GOFlags::Copy)
		{
			(*it_child)->flags = (*it_child)->flags & ~GOFlags::Copy;
			// TODO: Add copy option
		}
		if ((*it_child)->flags & GOFlags::Delete)
		{
			(*it_child)->flags = (*it_child)->flags & ~GOFlags::Delete;
			(*it_child)->CleanUp();
			//delete *it_child;
			children.erase(it_child++);
		}
		else
		{
			++it_child;
		}
	}
}

void GameObject::CleanUp()
{
	for (auto &component : components)
	{
		component->CleanUp();
	}

	for (auto &child : children)
	{
		child->CleanUp();
	}
}

#pragma region Game Object Related Functions
void GameObject::Draw()
{
	if (!active) return;
	if (transform == nullptr) return;

	for (const auto &child : children)
	{
		child->Draw();
	}
	// Set Shader and Texture
	unsigned program = App->renderer->programText;
	Texture* texture = nullptr;
	if (material != nullptr && material->active)
	{
		texture = material->texture;
	}

	//Draw meshes
	glUseProgram(program);

	math::float4x4 proj = App->camera->mainCamera->ProjectionMatrix();
	math::float4x4 view = App->camera->mainCamera->LookAt(App->camera->mainCamera->position + App->camera->mainCamera->front);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, (const float*)&GetGlobalTransform()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (const float*)&view);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, (const float*)&proj);

	if (mesh != nullptr && mesh->active)
	{
		((ComponentMesh*)mesh)->RenderMesh(App->renderer->programText, texture, view, proj);
	}
	glUseProgram(0);
}

void GameObject::DeleteGameObject()
{
	Unchild();
	CleanUp();
	delete this;
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
#pragma endregion

#pragma region Components Related Functions
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

void GameObject::DeleteComponent(Component* component)
{
	if (component != nullptr)
	{
		components.erase(components.begin() + component->GetComponentNumber());
		component->CleanUp();
		RELEASE(component);
	}
	else
		LOG("Warning: Component was nullptr.");
}
#pragma endregion

#pragma region Children Related Functions
void GameObject::Unchild()
{
	if (parent == nullptr)
	{
		LOG("Warning: %s doesn't have a parent.", name);
		return;
	}
	parent->children.remove(this);
}
#pragma endregion
