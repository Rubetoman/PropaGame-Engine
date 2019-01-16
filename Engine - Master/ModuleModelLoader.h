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

	bool            Init();
	update_status   Update();
	bool            CleanUp();
	bool LoadMesh(const char* path);
	void GenerateNodeMeshData(const char* path, const aiScene* scene, const aiNode* node, const aiMatrix4x4& parent_transform, GameObject* parent);
	void CreateGameObjectFromFile(const char& name);

	// Parshapes functions
	void LoadGeometry(GameObject* parent, Geometry_type geometryType);
	// Shape Creation
	//GameObject* CreateSphere(const char* name, const math::float3& position, const math::Quat& rotation, const math::float3& scale,
	//	unsigned slices, unsigned stacks, const math::float4& color);
	//GameObject* CreateCylinder(const char* name, const math::float3& position, const math::Quat& rotation, float height,
	//	float radius, unsigned slices, unsigned stacks, const math::float4& color);

public:

	const aiScene* scene = nullptr;


};
#endif /* __MODULEMODELLOADER_H__ */
