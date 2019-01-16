#ifndef __MODULEMODELLOADER_H__
#define __MODULEMODELLOADER_H__

#include "Module.h"

#include "Math/float3.h"
#include "Math/float4.h"
#include <assimp/matrix4x4.h>
#include <vector>
#include <iterator>

class GameObject;
class aiScene;
class aiNode;
class aiMesh;

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
