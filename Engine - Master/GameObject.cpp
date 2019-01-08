#include "GameObject.h"
#include "Globals.h"

#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleScene.h"
#include "ModuleResources.h"

#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentLight.h"
#include "ComponentCamera.h"

#include "debugdraw.h"

GameObject::GameObject(const char* name) : name(name)
{
	uuid = App->resources->GenerateNewUID();
	CreateComponent(component_type::Transform);
}

GameObject::GameObject(const char* name, GameObject* parent) : name(name), parent(parent)
{
	uuid = App->resources->GenerateNewUID();
	parentUID = parent->uuid;
	CreateComponent(component_type::Transform);
	parent->children.push_back(this);
}

GameObject::GameObject(const char* name, const math::float4x4& new_transform) : name(name)
{
	uuid = App->resources->GenerateNewUID();
	transform = (ComponentTransform*)CreateComponent(component_type::Transform);
	transform->SetTransform(new_transform);
}

GameObject::GameObject(const char* name, const math::float4x4& new_transform, GameObject* parent) 
	: name(name), parent(parent)
{
	uuid = App->resources->GenerateNewUID();
	parentUID = parent->uuid;
	transform = (ComponentTransform*)CreateComponent(component_type::Transform);
	transform->SetTransform(new_transform);
	parent->children.push_back(this);
}

GameObject::GameObject(const GameObject& go)
{
	uuid = App->resources->GenerateNewUID();
	parentUID = go.parentUID;
	name = go.name;
	static_GO = go.static_GO;

	for (const auto& component : go.components)
	{
		Component *new_comp = component->Duplicate();
		new_comp->my_go = this;
		new_comp->my_go_uid = uuid;
		components.push_back(new_comp);
		if (new_comp->type == component_type::Transform)
		{
			transform = (ComponentTransform*)new_comp;
		}
		else if (new_comp->type == component_type::Mesh)
		{
			mesh = (ComponentMesh*)new_comp;
		}
		else if (new_comp->type == component_type::Material)
		{
			material = (ComponentMaterial*)new_comp;
		}
		else if (new_comp->type == component_type::Camera)
		{
			App->resources->cameras.push_back((ComponentCamera*)new_comp);
		}
	}
	for (const auto& child : go.children)
	{
		GameObject* new_child = new GameObject(*child);
		new_child->parentUID = this->uuid;
		new_child->parent = this;
		children.push_back(new_child);
	}
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

void GameObject::Init()
{
	for (auto component : components)
		component->Init();

	for(auto child : children)
		child->Init();
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

bool GameObject::isActive() const
{
	if (active == false || parent == nullptr)
		return active;
	else
		return parent->isActive();
}

void GameObject::SetChildrenStatic(bool set) const
{
	for (auto child : children)
	{
		child->static_GO = set;
		child->SetChildrenStatic(set);
	}
}

void GameObject::SetForeparentStatic(bool set) const
{
	if (parent != nullptr && parent != App->scene->root)
	{
		parent->static_GO = set;
		parent->SetForeparentStatic(set);
	}
}

void GameObject::Draw(const math::float4x4& view, const math::float4x4& proj, ComponentCamera& camera)
{
	if (!active) return;
	if (transform == nullptr) return;

	for (const auto &child : children)
	{
		child->Draw(view, proj, camera);
	}

	// Compute and Draw BBox on Editor
	AABB boundingBox = ComputeBBox();
	if ((App->editor->hierarchy->selected == this) || (App->editor->drawAllBBox))
		DrawBBox(boundingBox);

	if (mesh != nullptr && mesh->active)
	{
		// Avoid drawing mesh if it is not inside frustum
		if (!camera.frustum_culling || camera.ContainsAABB(boundingBox))
			((ComponentMesh*)mesh)->RenderMesh(view, proj);
	}

	// Draw a sphere on Editor
	if (GetComponent(component_type::Light) != nullptr)
	{
		dd::sphere(transform->position, math::float3(1.0f, 1.0f, 1.0f), 0.2f);
	}

	// Draw a camera icon and Frustum on Editor
	ComponentCamera* camera_component = (ComponentCamera*)GetComponent(component_type::Camera);
	if (camera_component != nullptr)
	{
		dd::cone(transform->position + (camera_component->frustum.front * 0.3f), 0.5f * camera_component->frustum.front, math::float3(1.0f, 1.0f, 1.0f), 0.2f, 0.01f);
		dd::box(transform->position, math::float3(1.0f, 1.0f, 1.0f), 0.4f, 0.4f, 0.4f);

		dd::frustum((camera_component->frustum.ProjectionMatrix() * camera_component->frustum.ViewMatrix()).Inverted(), dd::colors::Purple);
	}
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

math::float3 GameObject::GetCenter() const
{
	if (mesh != nullptr)
		return mesh->boundingBox.CenterPoint();
	else
		return transform->position;
}

math::AABB GameObject::ComputeBBox() const 
{
	AABB bbox;

	// TODO: Solve bugs and use pointers
	bbox.SetNegativeInfinity();

	// Current GO meshes
	if (mesh != nullptr)
		bbox.Enclose(mesh->boundingBox);

	// Apply transformation of our GO
	bbox.TransformAsAABB(GetGlobalTransform());

	return bbox;
}

math::AABB GameObject::ComputeTotalBBox() const
{
	AABB bbox;

	// TODO: Solve bugs and use pointers
	bbox.SetNegativeInfinity();

	// Current GO meshes
	if (mesh != nullptr)
		bbox.Enclose(mesh->boundingBox);

	// Apply transformation of our GO
	bbox.TransformAsAABB(GetGlobalTransform());

	for (const auto &child : children)
	{
		bbox.Enclose(child->ComputeTotalBBox());
	}

	return bbox;
}

void GameObject::DrawBBox(AABB bbox) const 
{
	if(mesh != nullptr)
		dd::aabb(bbox.minPoint, bbox.maxPoint, math::float3(255, 255, 0), true);

	for (auto child : children)
		child->DrawBBox(child->ComputeTotalBBox());
}

void GameObject::DrawTotalBBox(AABB bbox) const
{
	dd::aabb(bbox.minPoint, bbox.maxPoint, math::float3(255, 255, 0), true);
}
#pragma endregion

#pragma region Components Related Functions
Component* GameObject::CreateComponent(component_type type)
{
	Component* component = nullptr;

	switch (type)
	{
	case component_type::Transform:
		if (transform == nullptr)
		{
			component = new ComponentTransform(this);
			transform = (ComponentTransform*)component;
		}
		else
		{
			LOG("Warning: %s already has a Transform Component attached.", name.c_str());
		}
		break;
	case component_type::Mesh:
		if (mesh == nullptr)
		{
			component = new ComponentMesh(this);
			mesh = (ComponentMesh*)component;
		}
		else
		{
			LOG("Warning: %s already has a Mesh Component attached.", name.c_str());
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
			LOG("Warning: %s already has a Material Component attached.", name.c_str());
		}
		break;
	case component_type::Light:
		if (GetComponents(component_type::Light).size() == 0)
		{
			component = new ComponentLight(this);
			if (App!=nullptr)
				App->resources->lights.push_back(this);
		}
		else
		{
			LOG("Warning: %s already has a Light Component attached.", name.c_str());
		}
		break;
	case component_type::Camera:
		if (GetComponents(component_type::Camera).size() == 0)
		{
			component = new ComponentCamera(this);
			if (App != nullptr)
			{
				App->resources->cameras.push_back((ComponentCamera*)component);
			}
		}
		else
		{
			LOG("Warning: %s already has a Camera Component attached.", name.c_str());
		}
		break;
	case component_type::Editor_Camera:
		if (App->camera->editor_camera_comp == nullptr)
		{
			component = new ComponentCamera(this);
			component->type = component_type::Editor_Camera;
		}
		else
		{
			LOG("Warning: %s already has a Camera Editor Component attached.", name.c_str());
		}
		break;
	default:
		break;
	}

	if(component != nullptr)
		components.push_back(component);

	return component;
}

Component* GameObject::GetComponent(component_type type) const
{
	for (auto &component : components)
	{
		if (component != nullptr && component->type == type)
		{
			return component;
		}
	}
	//LOG("Warning: %s doesn't have a %s component.", name, type);
	return nullptr;
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
		int position = component->GetComponentNumber();
		if (position > -1)
		{
			components.erase(components.begin() + position);
			component->CleanUp();
			RELEASE(component);
		}
		else
			LOG("Error deleting component.");
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
		LOG("Warning: %s doesn't have a parent.", name.c_str());
		return;
	}
	parent->children.remove(this);
}

void GameObject::SetParent(GameObject* new_parent)
{
	if (new_parent == nullptr)
		return;

	parentUID = new_parent->uuid;
	new_parent->children.push_back(this);

	// Set transform to global
	if (this->transform != nullptr)
		this->transform->LocalToGlobal(this->GetGlobalTransform());

	this->parent->children.remove(this);
	this->parent = new_parent;

	// Adapt to new parent transformation
	if (this->transform != nullptr)
		this->transform->GlobalToLocal(this->parent->GetGlobalTransform());
}

bool GameObject::isForefather(GameObject& go)
{
	if (parent == nullptr)
		return false;
	else if (&go == parent)
		return true;
	else
		return parent->isForefather(go);
}
#pragma endregion

#pragma region JSON

void GameObject::Save(JSON_value* go)
{
	JSON_value* gameObject = go->createValue();

	gameObject->AddString("UID", uuid.c_str());
	gameObject->AddString("ParentUID", parentUID.c_str());
	gameObject->AddString("Name", name.c_str());
	gameObject->AddBool("Active", active);
	gameObject->AddBool("Static", static_GO);

	JSON_value* Components = go->createValue();
	Components->convertToArray();
	for (std::vector<Component*>::iterator it_c = components.begin(); it_c != components.end(); it_c++)
	{

		(*it_c)->Save(Components);
	}

	gameObject->addValue("Components", Components);

	go->addValue("", gameObject);

	for (std::list<GameObject*>::iterator it_c = children.begin(); it_c != children.end(); it_c++)
	{
		(*it_c)->Save(go);
	}
}

void GameObject::Load(JSON_value* go)
{
	uuid = go->GetString("UID");
	parentUID = go->GetString("ParentUID");
	name = go->GetString("Name");
	active = go->GetBool("Active");
	static_GO = go->GetBool("Static");

	JSON_value* Components = go->getValue("Components"); //It is an array of values
	if (Components->getRapidJSONValue()->IsArray()) //Just make sure
	{
		for (int i = 0; i < Components->getRapidJSONValue()->Size(); i++)
		{
			JSON_value* componentData = Components->getValueFromArray(i); //Get the component data

			// If the component is a Transform just edit variables (because a GO is created with a Transform by default)
			if ((component_type)componentData->GetInt("Type") == component_type::Transform && transform != nullptr)
			{
				transform->Load(componentData); //Load its info
			}
			else // If no create it as a new component
			{
				Component* component = CreateComponent((component_type)componentData->GetInt("Type"));
				component->Load(componentData); //Load its info
			}
		}
	}
}

#pragma endregion