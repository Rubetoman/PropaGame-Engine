#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "MathGeoLib.h"

#include "JSON.h"
#include <string>
#include <list>

#define GO_NAME_SIZE 24
#define GO_DEFAULT_NAME "GameObject"

class Component;
class ComponentTransform;
class ComponentMesh;
class ComponentMaterial;
class ComponentCamera;

enum class component_type;
enum class BBoxMode;

#pragma region GOFlags
enum GOFlags
{
	None = 0,
	Delete = 1,
	Copy = 2,
	Duplicate = 3
};

inline GOFlags operator|(GOFlags a, GOFlags b)
{
	return static_cast<GOFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline GOFlags operator&(GOFlags a, GOFlags b)
{
	return static_cast<GOFlags>(static_cast<int>(a) & static_cast<int>(b));
}

inline GOFlags operator ~(GOFlags a)
{
	return static_cast<GOFlags>(~static_cast<int>(a));
}

inline GOFlags& operator |=(GOFlags &a, GOFlags b)
{
	a = static_cast<GOFlags> (static_cast<int>(a) | static_cast<int>(b));
	return a;
}
#pragma endregion

class GameObject
{
public:
	GameObject(const char* name);
	GameObject(const char* name, GameObject* parent);
	GameObject(const char* name, const math::float4x4& new_transform);
	GameObject(const char* name, const math::float4x4& new_transform, GameObject* parent);
	GameObject::GameObject(const GameObject& go);
	~GameObject();

	void Init();
	void Update();
	void CleanUp();

	// Game Object
	bool isActive() const;
	void SetStatic(bool set);		// Sets the GO to static and sets the scene dirty
	bool isPureStatic() const;		// Returns true if all his parents are static, if else returns false
	void SetChildrenStatic(bool set) const;
	void SetForeparentStatic(bool set) const;
	void Draw(const math::float4x4& view, const math::float4x4& proj, ComponentCamera& camera);
	void DrawDebugShapes(math::AABB bbox, BBoxMode bbox_mode) const;		// Draws editor debug shpaes (Spheres for lights, camera icons, frustrum lines, bboxes, etc.)
	void DeleteGameObject();
	math::float4x4 GetLocalTransform() const;
	math::float4x4 GetGlobalTransform() const;
	math::float3 GetCenter() const;

	// BBox
	math::AABB ComputeBBox() const;				// Computes a bbox for the GO mesh
	math::AABB ComputeTotalBBox() const;		// Computes a bbox enclosing all children meshes
	math::AABB ComputeStaticTotalBBox() const;	// Computes a bbox enclosing all static children meshes
	void DrawBBox(AABB bbox) const;				// Draws one bbox for each mesh (self and children)
	void DrawTotalBBox(AABB bbox) const;		// Draws one bbox

	// Components
	Component* CreateComponent(component_type type);
	Component* GetComponent(component_type type) const;
	std::vector<Component*> GetComponents(component_type type) const;
	void DeleteComponent(Component* component);

	// Children
	void Unchild();
	void SetParent(GameObject* new_parent);
	bool isForefather(GameObject& go);

	// JSON
	void Save(JSON_value* gameobject);
	void Load(JSON_value* go);

public:

	std::string uuid = "";
	std::string parentUID = "";
	bool active = true;
	bool static_GO = false;
	std::string name = "GameObject";
	GOFlags flags = GOFlags::None;
	
	// Hierarchy
	GameObject* parent = nullptr;
	std::list<GameObject*> children;

	// Components
	ComponentTransform* transform = nullptr;
	ComponentMesh* mesh_comp = nullptr;
	ComponentMaterial* material_comp = nullptr;
	std::vector<Component*> components;
};

#endif /*__GAMEOBJECT_H__*/