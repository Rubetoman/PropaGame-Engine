#include "GameObject.h"
#include "Globals.h"

#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleScene.h"
#include "ModuleResources.h"
#include "ModuleEditor.h"

#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentLight.h"
#include "ComponentCamera.h"

#include "GL/glew.h"
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
	if (parent->static_GO && parent != App->scene->root) SetStatic(true);
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
	if (parent->static_GO && parent != App->scene->root) SetStatic(true);
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
			mesh_comp = (ComponentMesh*)new_comp;
		}
		else if (new_comp->type == component_type::Material)
		{
			material_comp = (ComponentMaterial*)new_comp;
		}
		else if (new_comp->type == component_type::Camera)
		{
			App->resources->cameras.push_back((ComponentCamera*)new_comp);
		}
		else if (new_comp->type == component_type::Light)
		{
			App->resources->lights.push_back((ComponentLight*)new_comp);
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

	components.clear();
	children.clear();

	transform = nullptr;
	mesh_comp = nullptr;
	material_comp = nullptr;
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
			delete *it_child;
			children.erase(it_child++);
			App->scene->SetSceneDirty(true);
		}
		else
		{
			++it_child;
		}
	}

	for (auto component : components)
		component->Update();
}

void GameObject::CleanUp()
{
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

void GameObject::SetStatic(bool set)
{
	static_GO = set;
	App->scene->quadtree_dirty = true;
	App->scene->SetSceneDirty(true);
}

bool GameObject::isPureStatic() const
{
	if (parent == nullptr && static_GO)
		return true;
	else if (!static_GO || !parent->static_GO)
		return false;
	else
		return parent->isPureStatic();
}

void GameObject::SetChildrenStatic(bool set) const
{
	for (auto child : children)
	{
		child->SetStatic(set);
		child->SetChildrenStatic(set);
	}
}

void GameObject::SetForeparentStatic(bool set) const
{
	if (parent != nullptr && parent != App->scene->root)
	{
		parent->SetStatic(set);
		parent->SetForeparentStatic(set);
	}
}

void GameObject::Draw(const math::float4x4& view, const math::float4x4& proj, ComponentCamera& camera)
{
	if (!active) return;
	if (transform == nullptr) return;

	// Compute BBox
	AABB boundingBox;
	BBoxMode bbox_mode = App->editor->bbox_mode;

	if(bbox_mode == BBoxMode::Divide || bbox_mode == BBoxMode::Divide)
		boundingBox = ComputeBBox();
	else
		boundingBox = ComputeTotalBBox();

	// Draw debug shapes on editor camera
	if (&camera == App->camera->editor_camera_comp)
		DrawDebugShapes(boundingBox, bbox_mode);

	if (App->scene->use_quadtree && isPureStatic()) return;	// Static GOs meshes are drawn using quadtree

	if (mesh_comp != nullptr && mesh_comp->active)
	{
		// Avoid drawing mesh if it is not inside frustum
		if (!camera.frustum_culling || camera.ContainsAABB(boundingBox))
			((ComponentMesh*)mesh_comp)->RenderMesh(view, proj);
	}
}

void GameObject::DrawDebugShapes(math::AABB bbox, BBoxMode bbox_mode) const
{
	// Draw bbox
	switch (bbox_mode)
	{
	default:
	case BBoxMode::Divide:
		if (App->editor->selectedGO == this)
			DrawBBox(bbox);
		break;
	case BBoxMode::All_Divide:
		DrawBBox(bbox);
		break;
	case BBoxMode::Enclose:
		if (App->editor->selectedGO == this)
			DrawTotalBBox(bbox);
		break;
	case BBoxMode::All_Enclose:
		DrawTotalBBox(bbox);
		break;
	}

	float scale = App->editor->scale;

	// Draw a sphere on Editor
	ComponentLight* light = (ComponentLight*)GetComponent(component_type::Light);
	if (light != nullptr)
	{
		if (light->type == light_type::Point)
			dd::sphere(transform->position, light->color, light->GetAttenuationDistance());
		else if (light->type == light_type::Spot)
			dd::cone(transform->position, light->direction * light->GetAttenuationDistance(), light->color, light->GetAttenuationDistance() * tanf(math::DegToRad(light->outer_cutoff)), 0.0f);
		else
		{
			math::float3 pos = transform->position;
			math::float3 dir = light->direction;
			float scale = App->editor->scale;
			dd::circle(pos, light->direction, light->color, App->editor->scale, 5.0f);
			//TODO: Add lines
			/*dd::line(math::float3(pos.x + scale, pos.y, pos.z), math::float3(dir.x + scale, dir.y, dir.z + 5.0f * scale), light->color, 2 * scale);
			dd::line(math::float3(pos.x - scale, pos.y, pos.z), math::float3(pos.x - scale, pos.y, pos.z + 5.0f * scale), light->color, 2 * scale);
			dd::line(math::float3(pos.x + scale * 0.5f, pos.y + scale, pos.z), math::float3(pos.x + scale * 0.5f, pos.y + scale, pos.z + 5.0f * scale), light->color, 2 * scale);
			dd::line(math::float3(pos.x + scale * 2.0f, pos.y + scale, pos.z), math::float3(pos.x + scale * 2.0f, pos.y + scale, pos.z + 5.0f * scale), light->color, 2 * scale);
			dd::line(math::float3(pos.x, pos.y - scale, pos.z), math::float3(pos.x, pos.y - scale, pos.z + 5.0f * scale), light->color, 2 * scale);*/
		}
	}

	// Draw a camera icon and Frustum on Editor
	ComponentCamera* camera_component = (ComponentCamera*)GetComponent(component_type::Camera);
	if (camera_component != nullptr)
	{
		dd::cone(transform->position + (camera_component->frustum.front * scale * 0.3f), scale * 0.5f * camera_component->frustum.front, math::float3(1.0f, 1.0f, 1.0f), scale * 0.2f, scale * 0.01f);
		dd::box(transform->position, math::float3(1.0f, 1.0f, 1.0f), scale * 0.4f, scale * 0.4f, scale * 0.4f);

		dd::frustum((camera_component->frustum.ProjectionMatrix() * camera_component->frustum.ViewMatrix()).Inverted(), dd::colors::Purple);
	}
}

void GameObject::DeleteGameObject()
{
	Unchild();
	CleanUp();
	App->scene->SetSceneDirty(true);
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
	if (mesh_comp != nullptr)
		return ComputeBBox().CenterPoint();
	else
		return transform->position;
}

void GameObject::UpdateModel(unsigned shader) const
{
	glUniformMatrix4fv(glGetUniformLocation(shader,"model"), 1, GL_TRUE, &GetGlobalTransform()[0][0]);
}

#pragma endregion

#pragma region BBox functions

math::AABB GameObject::ComputeBBox() const 
{
	AABB bbox;

	// TODO: Solve bugs and use pointers
	bbox.SetNegativeInfinity();

	// Current GO meshes
	if (mesh_comp != nullptr)
		bbox.Enclose(mesh_comp->mesh.boundingBox);

	// Apply transformation of our GO
	bbox.TransformAsAABB(GetGlobalTransform());

	return bbox;
}

math::AABB GameObject::ComputeTotalBBox() const
{
	AABB bbox;

	// TODO: Solve bugs and use pointers
	bbox.SetNegativeInfinity();

	// Enclose GO meshes if is not only small box 
	if (mesh_comp != nullptr)
	{
		bbox.Enclose(mesh_comp->mesh.boundingBox);
	}
	else if (children.size() == 0)
	{
		bbox.maxPoint = math::float3(0.1f, 0.1f, 0.1f);
		bbox.minPoint = math::float3(-0.1f, -0.1f, -0.1f);
	}

	// Apply transformation of our GO
	bbox.TransformAsAABB(GetGlobalTransform());

	for (const auto &child : children)
	{
		bbox.Enclose(child->ComputeTotalBBox());
	}

	return bbox;
}

math::AABB GameObject::ComputeStaticTotalBBox() const
{
	AABB bbox;

	// TODO: Solve bugs and use pointers
	bbox.SetNegativeInfinity();

	// Enclose GO meshes if is not only small box 
	if (static_GO)
	{
		if (mesh_comp != nullptr)
		{
			bbox.Enclose(mesh_comp->mesh.boundingBox);
		}
		else
		{
			bbox.maxPoint = math::float3(0.1f, 0.1f, 0.1f);
			bbox.minPoint = math::float3(-0.1f, -0.1f, -0.1f);
		}

		// Apply transformation of our GO
		bbox.TransformAsAABB(GetGlobalTransform());

		for (const auto &child : children)
		{
			if(child->static_GO)
				bbox.Enclose(child->ComputeStaticTotalBBox());
		}
	}

	return bbox;
}

void GameObject::DrawBBox(AABB bbox) const 
{
	if(mesh_comp != nullptr)
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
		if (mesh_comp == nullptr)
		{
			component = new ComponentMesh(this);
			mesh_comp = (ComponentMesh*)component;
		}
		else
		{
			LOG("Warning: %s already has a Mesh Component attached.", name.c_str());
		}
		break;
	case component_type::Material:
		if (material_comp == nullptr)
		{
			component = new ComponentMaterial(this);
			material_comp = (ComponentMaterial*)component;
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
				App->resources->lights.push_back((ComponentLight*)component);
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

	if (component != nullptr)
	{
		components.push_back(component);
		App->scene->SetSceneDirty(true);
	}

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

void GameObject::DeleteComponent(Component& component)
{
	for (int i = 0; i < components.size(); ++i)
	{
		if (components[i] == &component)
		{
			RELEASE(components[i]);
			components.erase(components.begin() + i);
			App->scene->SetSceneDirty(true);
			return;

		}
	}
}
#pragma endregion

void GameObject::SetLightUniforms(unsigned shader) const
{
	ComponentLight* directional = App->resources->GetDirectionalLight();
	if (directional != nullptr)
	{
		glUniform3fv(glGetUniformLocation(shader, "lights.directional.direction"), 1, (GLfloat*)&directional->direction);
		glUniform3fv(glGetUniformLocation(shader, "lights.directional.color"), 1, (GLfloat*)&directional->color);
	}
	else
	{
		float3 noDirectional = float3::zero;
		glUniform3fv(glGetUniformLocation(shader, "lights.directional.direction"), 1, (GLfloat*)&noDirectional);
	}

	int i = 0;
	for (const auto & spot : App->scene->GetClosestLights(light_type::Spot, transform->position))
	{
		char buffer[32];

		sprintf(buffer, "lights.spots[%d].position", i);
		glUniform3fv(glGetUniformLocation(shader, buffer), 1, (GLfloat*)&spot->my_go->transform->position);

		memset(buffer, 0, 32);
		sprintf(buffer, "lights.spots[%d].direction", i);
		glUniform3fv(glGetUniformLocation(shader, buffer), 1, (GLfloat*)&spot->direction);

		memset(buffer, 0, 32);
		sprintf(buffer, "lights.spots[%d].color", i);
		glUniform3fv(glGetUniformLocation(shader, buffer), 1, (GLfloat*)&spot->color);

		memset(buffer, 0, 32);
		sprintf(buffer, "lights.spots[%d].attenuation", i);
		glUniform3fv(glGetUniformLocation(shader, buffer), 1, (GLfloat*)&spot->attenuation);

		memset(buffer, 0, 32);
		float innerRad = cosf(math::DegToRad(spot->inner_cutoff));
		sprintf(buffer, "lights.spots[%d].inner", i);
		glUniform1fv(glGetUniformLocation(shader,buffer), 1, (GLfloat*)&innerRad);

		memset(buffer, 0, 32);
		float outerRad = cosf(math::DegToRad(spot->outer_cutoff));
		sprintf(buffer, "lights.spots[%d].outer", i);
		glUniform1fv(glGetUniformLocation(shader,buffer), 1, (GLfloat*)&outerRad);

		++i;
	}
	glUniform1i(glGetUniformLocation(shader, "lights.num_spots"), i);

	i = 0;
	for (const auto & point : App->scene->GetClosestLights(light_type::Point, transform->position))
	{
		char buffer[32];

		sprintf(buffer, "lights.points[%d].position", i);
		glUniform3fv(glGetUniformLocation(shader, buffer), 1, (GLfloat*)&point->my_go->transform->position);

		memset(buffer, 0, 32);
		sprintf(buffer, "lights.points[%d].direction", i);
		glUniform3fv(glGetUniformLocation(shader, buffer), 1, (GLfloat*)&point->direction);

		memset(buffer, 0, 32);
		sprintf(buffer, "lights.points[%d].color", i);
		glUniform3fv(glGetUniformLocation(shader, buffer), 1, (GLfloat*)&point->color);

		memset(buffer, 0, 32);
		sprintf(buffer, "lights.points[%d].attenuation", i);
		glUniform3fv(glGetUniformLocation(shader, buffer), 1, (GLfloat*)&point->attenuation);

		++i;
	}
	glUniform1i(glGetUniformLocation(shader, "lights.num_points"), i);
}

#pragma region Children Related Functions
void GameObject::Unchild()
{
	if (parent == nullptr)
	{
		LOG("Warning: %s doesn't have a parent.", name.c_str());
		return;
	}
	parent->children.remove(this);
	App->scene->SetSceneDirty(true);
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

	App->scene->SetSceneDirty(true);
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