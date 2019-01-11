#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__

#include "Importer.h"
#include <assimp/matrix4x4.h>

class aiScene;
class aiNode;
class aiMesh;

class GameObject;

class MeshImporter : public Importer
{
public:
	MeshImporter();
	~MeshImporter();

	bool Import(const char* file, const char* path, std::string& output_file) override;
	void SaveScene(const aiScene* scene);
	void SaveNode(const aiScene& scene, const aiNode& node, const aiMatrix4x4& parent_transform, GameObject* parent);
	void SaveMesh(const aiMesh& mesh);
};

#endif /*__MESHIMPORTER_H__*/