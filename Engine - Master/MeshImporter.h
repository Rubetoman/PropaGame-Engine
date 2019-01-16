#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__

#include <assimp/matrix4x4.h>

class aiScene;
class aiNode;
class aiMesh;

class GameObject;
struct Mesh;

class MeshImporter
{
public:
	MeshImporter();
	~MeshImporter();

	static void ImportFBX(const char* filePath);
	static bool Import(const aiMesh* aiMesh, const char* meshName, GameObject* parent);
	static bool Save(const Mesh& mesh, const char* meshName);
	static bool Load(Mesh* mesh, const char* meshName);
	static void CleanUpMesh(Mesh* mesh);
};

#endif /*__MESHIMPORTER_H__*/