#ifndef __MODULESCENE_H__
#define __MODULESCENE_H__

#include "Module.h"
#include "ImGuizmo/ImGuizmo.h"
#include "Math/float4x4.h"
#include <vector>

class GameObject;
class ComponentCamera;
class JSON_file;
class Quadtree;

class ModuleScene : public Module
{
public:
	ModuleScene();
	~ModuleScene();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate() override;
	bool CleanUp() override;

	void Draw(const math::float4x4& view, const math::float4x4& proj, ComponentCamera& camera);
	void DrawStaticGameObjects(const math::float4x4& view, const math::float4x4& proj, ComponentCamera& camera) const;
	void DrawImGuizmo(ImGuizmo::OPERATION operation) const;

	GameObject* CreateGameObject(const char* name);
	GameObject* CreateGameObject(const char* name, GameObject* parent);
	GameObject* CreateGameObject(const char* name, math::float4x4& transform);
	GameObject* CreateGameObject(const char* name, math::float4x4& transform, GameObject* parent);

	void DeleteGameObject(GameObject* go);
	GameObject* DuplicateGameObject(const GameObject* go);
	void Unchild(GameObject* go);

	unsigned GetSceneGONumber(GameObject& go) const;

	bool Save(JSON_file* document);

	bool InitScene();						// Creates a scene with root, default light and game camera
	void NewScene(bool init);				// True to Init scene after deleting scene
	bool SaveScene(const char* scene_name);
	bool LoadScene(const char* scene_name);
	bool DeleteScene(const char* scene_name);
	bool SetSceneDirty(bool set);	

	void FillQuadtree(GameObject* go);
	void ComputeSceneQuadtree();
	void ResizeQuadtree(GameObject* go);

public:
	std::vector<GameObject*> scene_gos;	// Vector with pointers to all scene GOs
	GameObject* root = nullptr;			// Root GO, parent of every GO created on the scene
	bool show_scene_gos = false;		// If true hided engine GOs will appear on hierarchy (Root, Game Camera...)
	std::string name = "";				// Name of the scene
	bool save_scene = false;			// If true on next update the scene will be saved

	bool quadtree_dirty = false;		// True when a modification to a static GO has been made
	bool dirty = false;					// True when a modification has been made

	// Scene Quadtree
	Quadtree* quadtree = nullptr;
	bool draw_quadtree = false;
	bool use_quadtree = true;
	std::vector<GameObject*> static_gos; // static GOs that intersect with the camera frustrum
};

#endif /*__MODULESCENE_H__*/