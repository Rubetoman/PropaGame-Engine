#ifndef __MODULERESOURCES_H__
#define __MODULERESOURCES_H__

#include "Module.h"

#include <vector>
#include <map>
#include "Util/crossguid/guid.hpp"

class GameObject;
class Texture;
class ComponentCamera;

class ModuleResources : public Module
{
public:
	ModuleResources();
	~ModuleResources();

	bool Init() override;
	bool CleanUp() override;

	std::string GenerateNewUID();
	unsigned GetLightNumber(GameObject& go) const;

	// Cameras
	unsigned GetCameraNumber(GameObject& go) const;
	void DeleteCamera(ComponentCamera* go);

public:
	std::vector<GameObject*> lights;		// List of all the lights on the scene

	// Textures
	std::map<Texture*, unsigned> textures;	// Map of all the textures and number of objects that use that texture
	Texture* checkers_texture = nullptr;
	Texture* no_camera_texture = nullptr;

	// Scene Cameras
	std::vector<GameObject*> cameras;		// Vector with all the cameras on the scene
};

#endif /*__MODULERESOURCES_H__*/