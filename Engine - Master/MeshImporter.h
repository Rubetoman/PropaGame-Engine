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
	bool SaveScene(const aiScene& scene, std::string& output_file);
	void SaveNode(const aiNode& node, const aiScene& scene, char* &cursor, int node_id, int parent_node_id);
	void SaveMesh(const aiMesh& mesh, char* &cursor);

	unsigned int GetNodeSize(const aiNode& node, const aiScene& scene) const;
};

#endif /*__MESHIMPORTER_H__*/