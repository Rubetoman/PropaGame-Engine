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

	void ImportFBX(const char* filePath);
	bool Import(const aiMesh* aiMesh, const char* meshName);
	bool Save(const ComponentMesh& mesh, const char* meshName);
	bool Load(ComponentMesh* mesh, const char* meshName);
};

#endif /*__MESHIMPORTER_H__*/