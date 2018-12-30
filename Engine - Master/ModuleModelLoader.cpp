#include "ModuleModelLoader.h"
#include "ModuleCamera.h"
#include "ModuleScene.h"

#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "par_shapes.h"
#pragma warning(pop)

ModuleModelLoader::ModuleModelLoader()
{
}

ModuleModelLoader::~ModuleModelLoader()
{
}

bool ModuleModelLoader::Init()
{
	//CreateSphere("sphere", math::float3(0.0f, 0.0f, 0.0f), Quat::identity, math::float3(1.0f, 1.0f, 1.0f), 20, 20, float4(0.f, 0.0f, 0.5f, 1.0f));
	//CreateCylinder("cylinder0", math::float3(0.0f, 0.0f, 0.0f), Quat::identity, 2.0f, 1.0f, 30, 30, float4(0.f, 0.5f, 0.5f, 1.0f));

	return LoadMesh("Assets/Models/BakerHouse.fbx");
}

update_status ModuleModelLoader::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleModelLoader::CleanUp()
{
	// Delete loaded models
	/*for (unsigned i = 0u; i < meshes.size(); ++i)
	{
		DeleteMesh(i);
	}
	meshes.clear();*/

	/*for (unsigned i = 0u; i < materials.size(); ++i)
	{
		DeleteMaterial(i);
	}*/
	return true;
}

bool ModuleModelLoader::LoadMesh(const char* path)
{
	assert(path != nullptr);
	unsigned int postprocess_flags = aiProcessPreset_TargetRealtime_MaxQuality;
	scene = aiImportFile(path, postprocess_flags);
	if (scene == nullptr)
	{
		LOG("Error, the file couldn't be loaded: %s \n", aiGetErrorString());
		return false;
	}
	App->camera->BBtoLook = new AABB({ .0f, .0f, .0f }, { .0f, .0f, .0f });

	GenerateNodeMeshData(scene, scene->mRootNode, aiMatrix4x4(), App->scene->root);
	aiReleaseImport(scene);
	return true;
}

/*void ModuleModelLoader::LoadMaterial(const char* path)
{
	assert(path != nullptr);
	const aiMaterial* src_material = nullptr;
	material gen_material;

	aiString file;
	aiTextureMapping mapping;
	unsigned uvindex = 0;

	if (src_material->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, &uvindex) == AI_SUCCESS)
	{
		gen_material.texture0 = App->textures->loadTexture(file.data);
	}

	materials.push_back(gen_material);
}*/

void ModuleModelLoader::GenerateNodeMeshData(const aiScene* scene, const aiNode* node, const aiMatrix4x4& parent_transform, GameObject* parent)
{
	assert(scene != nullptr); assert(node != nullptr);

	aiMatrix4x4 transform = parent_transform * node->mTransformation;
	GameObject* go = App->scene->CreateGameObject(node->mName.C_Str(), (math::float4x4&)transform, parent);

	// Avoid creating GO without name or too long
	if (go->name.size() < 1 || node->mName.length > GO_NAME_SIZE)
		go->name = GO_DEFAULT_NAME;


	for (unsigned i = 0; i < node->mNumMeshes; ++i)
	{
		// Add Mesh Component
		ComponentMesh* mesh = (ComponentMesh*)go->CreateComponent(component_type::Mesh);

		const aiMesh* src_mesh = scene->mMeshes[node->mMeshes[i]];

		// vertex array objects (VAO)
		glGenVertexArrays(1, &mesh->vao);
		glBindVertexArray(mesh->vao);

		glGenBuffers(1, &mesh->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);

		// Divide Buffer for position and UVs
		glBufferData(GL_ARRAY_BUFFER, src_mesh->mNumVertices * (sizeof(float) * 3 + sizeof(float) * 2), nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * src_mesh->mNumVertices, src_mesh->mVertices);

		// Texture coords (UVs)
		// MapBufferRange because we only want UV data from UVW
		math::float2* texCoords = (math::float2*)glMapBufferRange(GL_ARRAY_BUFFER, sizeof(float) * 3 * src_mesh->mNumVertices,
			sizeof(float) * 2 * src_mesh->mNumVertices, GL_MAP_WRITE_BIT);

		for (unsigned j = 0; j < src_mesh->mNumVertices; ++j)
		{
			texCoords[j] = math::float2(src_mesh->mTextureCoords[0][j].x, src_mesh->mTextureCoords[0][j].y);
		}

		glUnmapBuffer(GL_ARRAY_BUFFER);

		// Indices (faces)
		glGenBuffers(1, &mesh->ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, src_mesh->mNumFaces * (sizeof(unsigned) * 3), nullptr, GL_STATIC_DRAW);

		// Texture coords (UVs)
		// MapBufferRange because we only want UV data from UVW
		unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0,
			sizeof(unsigned)*src_mesh->mNumFaces * 3, GL_MAP_WRITE_BIT);

		for (unsigned j = 0; j < src_mesh->mNumFaces; ++j)
		{
			assert(src_mesh->mFaces[j].mNumIndices == 3);

			*(indices++) = src_mesh->mFaces[j].mIndices[0];
			*(indices++) = src_mesh->mFaces[j].mIndices[1];
			*(indices++) = src_mesh->mFaces[j].mIndices[2];
		}

		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,                  // attribute 0
			3,                  // number of componentes (3 floats)
			GL_FLOAT,           // data type
			GL_FALSE,           // should be normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,                  // attribute 0
			2,                  // number of componentes (3 floats)
			GL_FLOAT,           // data type
			GL_FALSE,           // should be normalized?
			0,                  // stride
			(void*)(sizeof(float) * 3 * src_mesh->mNumVertices)       // array buffer offset
		);

		// Disable VAO
		glBindVertexArray(0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Disable VBO and EBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//gen_mesh->material = src_mesh->mMaterialIndex;
		mesh->num_vertices = src_mesh->mNumVertices;
		mesh->num_indices = src_mesh->mNumFaces * 3;

		mesh->boundingBox.SetNegativeInfinity();
		mesh->boundingBox.Enclose((math::float3*)src_mesh->mVertices, mesh->num_vertices);

		const aiMaterial* src_material = scene->mMaterials[src_mesh->mMaterialIndex];

		if (src_material != nullptr)
		{
			// Load material
			ComponentMaterial* material = (ComponentMaterial*)go->CreateComponent(component_type::Material);

			aiString file;
			aiTextureMapping mapping;
			unsigned uvindex = 0;

			if (src_material->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, &uvindex) == AI_SUCCESS)
			{
				go->material->diffuse_map = App->textures->loadTexture(file.data);
			}
			else
			{
				LOG("Couldn't read the texture from .fbx file");
			}
		}
		
	}
	if (go->mesh != nullptr)
	{
		App->camera->BBtoLook->Enclose(go->mesh->boundingBox);
		App->camera->FitCamera(*App->camera->BBtoLook);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		GenerateNodeMeshData(scene, node->mChildren[i], transform, go);
	}
}

void ModuleModelLoader::ChangeMeshTexture(const char * path)
{
	assert(path != nullptr);
	unsigned width, height;
	Texture* tex = App->textures->loadTexture(path);

	//App->textures->unloadTexture((*meshes.begin())->texture);

	/*for (std::vector<mesh*>::iterator it_m = meshes.begin(); it_m != meshes.end(); it_m++)
	{
		(*it_m)->texture = tex;
	}*/
}

GameObject* ModuleModelLoader::CreateSphere(const char* name, const math::float3& position, const math::Quat& rotation, const math::float3& scale,
	unsigned slices, unsigned stacks, const math::float4& color)
{
	par_shapes_mesh* mesh = par_shapes_create_parametric_sphere(int(slices), int(stacks));

	if (mesh == nullptr)
	{
		LOG("Error: Sphere couldn't be created. Par_shapes returned nullptr.");
		return nullptr;
	}

	par_shapes_scale(mesh, scale.x, scale.y, scale.z);
	
	GameObject* sphere = App->scene->CreateGameObject(name, float4x4::FromTRS(position, rotation, scale));
	
	ComponentMesh* sphere_mesh = (ComponentMesh*)sphere->CreateComponent(component_type::Mesh);
	sphere_mesh->GenerateMesh(mesh);

	par_shapes_free_mesh(mesh);

	ComponentMaterial* sphere_material = (ComponentMaterial*)sphere->CreateComponent(component_type::Material);
	sphere_material->diffuse_map = App->textures->loadTexture("Checkers_Texture.jpg");

	return sphere;
}

GameObject* ModuleModelLoader::CreateCylinder(const char* name, const math::float3& position, const math::Quat& rotation, float height,
	float radius, unsigned slices, unsigned stacks, const math::float4& color)
{
	par_shapes_mesh* mesh = par_shapes_create_parametric_sphere(int(slices), int(stacks));
	par_shapes_rotate(mesh, -float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(mesh, 0.0f, -0.5f, 0.0f);

	par_shapes_mesh* top = par_shapes_create_disk(radius, int(slices), (const float*)&math::float3::zero, (const float*)&math::float3::unitZ);
	par_shapes_rotate(top, -float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(top, 0.0f, height*0.5f, 0.0f);

	par_shapes_mesh* bottom = par_shapes_create_disk(radius, int(slices), (const float*)&math::float3::zero, (const float*)&math::float3::unitZ);
	par_shapes_rotate(bottom, float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(bottom, 0.0f, height*-0.5f, 0.0f);

	if (mesh == nullptr)
	{
		LOG("Error: Cylinder couldn't be created. Par_shapes returned nullptr.");
		return nullptr;
	}

	par_shapes_scale(mesh, radius, height, radius);
	par_shapes_merge_and_free(mesh, top);
	par_shapes_merge_and_free(mesh, bottom);

	GameObject* cylinder = App->scene->CreateGameObject(name, float4x4::FromTRS(position, rotation, math::float3::one));

	ComponentMesh* cylinder_mesh = (ComponentMesh*)cylinder->CreateComponent(component_type::Mesh);
	cylinder_mesh->GenerateMesh(mesh);

	par_shapes_free_mesh(mesh);

	ComponentMaterial* cylinder_material = (ComponentMaterial*)cylinder->CreateComponent(component_type::Material);
	cylinder_material->diffuse_map = App->textures->loadTexture("Checkers_Texture.jpg");

	return cylinder;
}