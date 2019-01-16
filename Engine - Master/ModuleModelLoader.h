#ifndef __MODULEMODELLOADER_H__
#define __MODULEMODELLOADER_H__

#include "Module.h"

class GameObject;

enum class Geometry_type {SPHERE, TORUS, PLANE, CUBE};

class ModuleModelLoader : public Module
{
public:
	ModuleModelLoader();
	~ModuleModelLoader();

	void CreateGameObjectFromFile(const char& name);

	// Parshapes shape creation
	void LoadGeometry(GameObject* parent, Geometry_type geometryType);
};
#endif /* __MODULEMODELLOADER_H__ */
