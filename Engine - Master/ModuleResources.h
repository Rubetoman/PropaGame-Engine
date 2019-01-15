#ifndef __MODULERESOURCES_H__
#define __MODULERESOURCES_H__

#include "Module.h"

#include <vector>
#include <map>
#include "Util/crossguid/guid.hpp"
#include "Math/float2.h"

class GameObject;
class Texture;
class ComponentCamera;
class ComponentMesh;

class ModuleResources : public Module
{
public:
	ModuleResources();
	~ModuleResources();

	bool Init() override;
	update_status Update() override;
	bool CleanUp() override;

	std::string GenerateNewUID();
	unsigned GetLightNumber(GameObject& go) const;

	// Cameras
	unsigned GetCameraNumber(ComponentCamera& camera) const;
	void DeleteCamera(ComponentCamera* camera);

	// Meshes
	unsigned GetMeshNumber(ComponentMesh& mesh) const;
	void DeleteMesh(ComponentMesh* mesh);

	// Own created files
	void UpdateMeshesList();
	void UpdateTexturesList();
	void UpdateScenesList();

	// FBX files
	void UpdateFBXMeshesList();

public:
	std::vector<GameObject*> lights;		// List of all the lights on the scene

	// Default textures
	//std::map<Texture*, int> textures;		// Map of all the textures and number of objects that use that texture
	unsigned checkers_texture = 0u;
	math::float2 checkers_size = math::float2::zero;
	unsigned no_camera_texture = 0u;
	math::float2 no_camera_size = math::float2::zero;

	// Scene Cameras
	std::vector<ComponentCamera*> cameras;	// Vector with all the cameras on the scene

	// Meshes
	std::vector<ComponentMesh*> meshes;		// Vector of all the meshes on the scene

	//FBX files
	std::vector<std::string>* fbx_meshes = nullptr;

	// Own files
	std::vector<std::string>* file_meshes = nullptr;
	std::vector<std::string>* file_textures = nullptr;
	std::vector<std::string>* file_scenes = nullptr;

	bool itemToDelete = false;
	bool removeMesh = false;
};

#endif /*__MODULERESOURCES_H__*/