#include "MeshImporter.h"

#include "Globals.h"
#include "Application.h"

#include "ModuleScene.h"
#include "ModuleResources.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#include <assert.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>

MeshImporter::MeshImporter()
{
}


MeshImporter::~MeshImporter()
{
}

bool MeshImporter::Import(const char* file, const char* path, std::string& output_file)
{
	assert(path != nullptr);
	//If we already have models loaded, we erase them 
	//Only one model at a time for the moment
	const aiScene* scene = aiImportFile(path, aiProcess_Triangulate);
	if (scene != nullptr)
	{
		SaveScene(scene);
	}
	else
	{
		LOG("Error, the file couldn't be loaded: %s \n", aiGetErrorString());
		return false;
	}

	return true;
}

void MeshImporter::SaveScene(const aiScene* scene)
{
	assert(scene != nullptr);
	// Generate root node
	aiMatrix4x4 root_transform = aiMatrix4x4() * scene->mRootNode->mTransformation;
	GameObject* root_go = App->scene->CreateGameObject(scene->mRootNode->mName.C_Str(), (math::float4x4&)root_transform, App->scene->root);

	// Generate meshes as root GOs
	SaveNode(*scene, *scene->mRootNode, root_transform, root_go);

	aiReleaseImport(scene);
}

void MeshImporter::SaveNode(const aiScene& scene, const aiNode& node, const aiMatrix4x4& parent_transform, GameObject* parent)
{
	for (unsigned i = 0; i < node.mNumMeshes; ++i)
	{
		aiMatrix4x4 transform = parent_transform * node.mTransformation;
		GameObject* go = App->scene->CreateGameObject(node.mName.C_Str(), (math::float4x4&)transform, parent);

		// Avoid creating GO without name or too long
		if (go->name.size() < 1 || node.mName.length > GO_NAME_SIZE)
			go->name = GO_DEFAULT_NAME;

		// Add Mesh Component
		ComponentMesh* mesh = (ComponentMesh*)go->CreateComponent(component_type::Mesh);

		const aiMesh* src_mesh = scene.mMeshes[node.mMeshes[i]];

		// TODO: Create new mesh on ComponentMesh

		mesh->boundingBox.SetNegativeInfinity();
		mesh->boundingBox.Enclose((math::float3*)src_mesh->mVertices, mesh->num_vertices);
		App->resources->meshes.push_back(go->mesh);

		const aiMaterial* src_material = scene.mMaterials[src_mesh->mMaterialIndex];

		if (src_material != nullptr)
		{
			// Load material
			ComponentMaterial* material = (ComponentMaterial*)go->CreateComponent(component_type::Material);

			aiString file;
			aiTextureMapping mapping;
			unsigned uvindex = 0;

			// TODO: Create new material on ComponentMaterial
		}
	}

	if (node.mNumMeshes == 0)
	{
		for (unsigned int i = 0; i < node.mNumChildren; i++)
		{
			SaveNode(scene, *node.mChildren[i], parent_transform, parent);
		}
	}
}

void MeshImporter::SaveMesh(const aiMesh& mesh)
{
	unsigned int ranges[2] = { mesh.mNumFaces * 3, 	mesh.mNumVertices };
}