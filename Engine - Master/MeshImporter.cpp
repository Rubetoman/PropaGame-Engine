#include "MeshImporter.h"

#include "Globals.h"
#include "Application.h"

#include "ModuleScene.h"
#include "ModuleResources.h"
#include "ModuleFileManager.h"

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

void MeshImporter::ImportFBX(const char* filePath)
{
	char* fileBuffer;
	unsigned lenghBuffer = App->file->Load(filePath, &fileBuffer);
	const aiScene* scene = aiImportFileFromMemory(fileBuffer, lenghBuffer, aiProcessPreset_TargetRealtime_MaxQuality, "");

	std::string fileName;
	App->file->SplitPath(filePath, nullptr, &fileName, nullptr);

	if (scene != nullptr && scene->mMeshes != nullptr)
	{
		// Create root GO
		GameObject* root_go = App->scene->CreateGameObject(scene->mRootNode->mName.C_Str(), App->scene->root);

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			std::string meshName = fileName;
			meshName.append("_" + std::to_string(i));
			Import(scene->mMeshes[i], meshName.c_str(), root_go);
		}
		delete[] fileBuffer;
		fileBuffer = nullptr;
	}
	App->resources->UpdateMeshesList();

}

bool MeshImporter::Import(const aiMesh* aiMesh, const char* meshName, GameObject* parent) 
{
	bool result = false;

	if (aiMesh == nullptr)
	{
		LOG("Error: failed to import FBX file.");
		return result;
	}

	GameObject* go = App->scene->CreateGameObject(aiMesh->mName.C_Str(), parent);
	Mesh mesh;
	//Add Mesh Component
	ComponentMesh* mesh_comp = (ComponentMesh*)go->CreateComponent(component_type::Mesh);
	go->CreateComponent(component_type::Material);
	
	mesh.num_vertices = aiMesh->mNumVertices;
	mesh.vertices = new float[mesh.num_vertices * 3];
	memcpy(mesh.vertices, aiMesh->mVertices, sizeof(float) * mesh.num_vertices * 3);

	if (aiMesh->HasFaces()) 
	{
		mesh.num_indices = aiMesh->mNumFaces * 3;
		mesh.indices = new unsigned[mesh.num_indices];
		for (unsigned i = 0u; i < aiMesh->mNumFaces; ++i) 
		{
			memcpy(&mesh.indices[i * 3], aiMesh->mFaces[i].mIndices, 3 * sizeof(unsigned));
		}
	}

	if (aiMesh->HasTextureCoords(0)) 
	{
		mesh.uvs = new float[mesh.num_vertices * 2];
		int uvsCount = 0;
		for (unsigned i = 0u; i < mesh.num_vertices; i++)
		{
			mesh.uvs[uvsCount] = aiMesh->mTextureCoords[0][i].x;
			uvsCount++;
			mesh.uvs[uvsCount] = aiMesh->mTextureCoords[0][i].y;
			uvsCount++;
		}
	}

	if (aiMesh->HasNormals()) 
	{
		mesh.normals = new float[mesh.num_vertices * 3];
		memcpy(mesh.normals, aiMesh->mNormals, sizeof(float) * mesh.num_vertices * 3);
	}

	if (aiMesh->HasVertexColors(0)) {
		mesh.colors = new float[mesh.num_vertices * 3];
		memcpy(mesh.colors, aiMesh->mColors, sizeof(float) * mesh.num_vertices * 3);
	}

	mesh.boundingBox.SetNegativeInfinity();
	mesh.boundingBox.Enclose((math::float3*)aiMesh->mVertices, aiMesh->mNumVertices);

	mesh_comp->mesh = mesh;
	mesh_comp->ComputeMesh();

	result = Save(mesh, meshName);

	if (result != false)
	{
		std::string meshFile(meshName);

		meshFile.append(".proMesh");
		mesh_comp->currentMesh = meshFile;
	}

	return result;
}

bool MeshImporter::Save(const Mesh& mesh, const char* meshName)
{
	bool result = false;

	// indices / vertices / uvs / normals/ colors / 
	unsigned ranges[5] =
	{
		mesh.num_indices,
		mesh.num_vertices,
		(mesh.uvs) ? mesh.num_vertices : 0u,
		(mesh.normals) ? mesh.num_vertices : 0u,
		(mesh.colors) ? mesh.num_vertices : 0u
	};

	unsigned size = sizeof(ranges);
	size += sizeof(unsigned) * mesh.num_indices;
	size += sizeof(float) * mesh.num_vertices * 3;
	if (mesh.uvs)
	{
		size += sizeof(float) * mesh.num_vertices * 2;
	}
	if (mesh.normals)
	{
		size += sizeof(float) * mesh.num_vertices * 3;
	}
	if (mesh.colors)
	{
		size += sizeof(float) * mesh.num_vertices * 3;
	}

	char* data = new char[size];
	char* cursor = data;

	unsigned bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	bytes = sizeof(unsigned) * mesh.num_indices;
	memcpy(cursor, mesh.indices, bytes);
	cursor += bytes;

	bytes = sizeof(float) * mesh.num_vertices * 3;
	memcpy(cursor, mesh.vertices, bytes);
	cursor += bytes;

	if (mesh.uvs != nullptr)
	{
		bytes = sizeof(float)* mesh.num_vertices * 2;
		memcpy(cursor, mesh.uvs, bytes);
		cursor += bytes;
	}

	if (mesh.normals != nullptr)
	{
		bytes = sizeof(float)* mesh.num_vertices * 3;
		memcpy(cursor, mesh.normals, bytes);
		cursor += bytes;
	}

	if (mesh.colors != nullptr)
	{
		bytes = sizeof(float)* mesh.num_vertices * 3;
		memcpy(cursor, mesh.colors, bytes);
		cursor += bytes;
	}

	std::string fileToSave(MESHES_FOLDER);

	fileToSave.append(meshName);
	fileToSave.append(".proMesh");

	result = App->file->Save(fileToSave.c_str(), data, size, false);

	delete[] data;
	data = nullptr;

	return result;
}

bool MeshImporter::Load(Mesh* mesh, const char* meshName)
{
	bool result = false;

	if (mesh == nullptr) 
	{
		LOG("Error: failed to load FBX.");
		return result;
	}

	char* buffer;
	std::string meshPath(MESHES_FOLDER);
	meshPath.append(meshName);
	unsigned size = App->file->Load(meshPath.c_str(), &buffer);

	if (buffer != nullptr && size > 0) 
	{
		char* cursor = buffer;

		// indices / vertices / uvs / normals/ colors / 
		unsigned ranges[5];
		unsigned bytes = sizeof(ranges);
		memcpy(ranges, cursor, bytes);
		cursor += bytes;

		mesh->num_indices = ranges[0];
		mesh->num_vertices = ranges[1];

		// indices
		bytes = sizeof(unsigned) * mesh->num_indices;
		mesh->indices = new unsigned[mesh->num_indices];
		memcpy(mesh->indices, cursor, bytes);
		cursor += bytes;

		// vertices
		bytes = sizeof(float) * mesh->num_vertices * 3;
		mesh->vertices = new float[mesh->num_vertices * 3];
		memcpy(mesh->vertices, cursor, bytes);
		cursor += bytes;

		if (ranges[2] > 0) 
		{
			bytes = sizeof(float)* mesh->num_vertices * 2;
			mesh->uvs = new float[mesh->num_vertices * 2];
			memcpy(mesh->uvs, cursor, bytes);
			cursor += bytes;
		}

		if (ranges[3] > 0) 
		{
			bytes = sizeof(float)* mesh->num_vertices * 3;
			mesh->normals = new float[mesh->num_vertices * 3];
			memcpy(mesh->normals, cursor, bytes);
			cursor += bytes;
		}

		if (ranges[4] > 0) 
		{
			bytes = sizeof(float)* mesh->num_vertices * 3;
			mesh->colors = new float[mesh->num_vertices * 3];
			memcpy(mesh->colors, cursor, bytes);
			cursor += bytes;
		}

		delete[] buffer;
		buffer = nullptr;

		result = true;
	}

	return result;
}