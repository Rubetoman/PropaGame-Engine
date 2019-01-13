#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__

#include <assimp/matrix4x4.h>

class aiScene;
class aiNode;
class aiMesh;

class ComponentMesh;

class MeshImporter
{
public:
	MeshImporter();
	~MeshImporter();

	static void ImportFBX(const char* filePath);
	static bool Import(const aiMesh* aiMesh, const char* meshName);
	static bool Save(const ComponentMesh& mesh, const char* meshName);
	static bool Load(ComponentMesh* mesh, const char* meshName);
};

#endif /*__MESHIMPORTER_H__*/