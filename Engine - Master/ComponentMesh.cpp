#include "ComponentMesh.h"

#include "ComponentTransform.h"
#include "ComponentMaterial.h"

#include "Application.h"
#include "ModuleShader.h"
#include "ModuleResources.h"

#include "GameObject.h"
#include "par_shapes.h"
#include "GL/glew.h"

ComponentMesh::ComponentMesh(GameObject* go) : Component(go, component_type::Mesh)
{
}

ComponentMesh::ComponentMesh(const ComponentMesh& comp) : Component(comp)
{
	// TODO: Create new buffers or not delete when they are used by other comp
	vbo = comp.vbo;
	ibo = comp.ibo;
	vao = comp.vao;
	num_vertices = comp.num_vertices;
	vertices = comp.vertices;
	num_indices = comp.num_indices;
	indices = comp.indices;
	num_normals = comp.num_normals;
	normals = comp.normals;
	boundingBox = comp.boundingBox;
	App->resources->meshes.push_back(this);
}

ComponentMesh::~ComponentMesh()
{
}

Component* ComponentMesh::Duplicate()
{
	return new ComponentMesh(*this);
}

void ComponentMesh::CleanUp()
{
	Component::CleanUp();
	DeleteMesh();
}

bool ComponentMesh::DrawOnInspector()
{
	ImGui::PushID(this);
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Mesh Component"))
	{
		bool deleted = Component::DrawOnInspector();
		if (!deleted)
		{
			ImGui::Text("Triangles Count: %d", num_indices / 3);
			ImGui::Text("Vertices Count: %d", num_vertices);
		}
		else
		{
			ImGui::PopID();
			return deleted;
		}
	}
	ImGui::PopID();
	return false;
}

void ComponentMesh::GenerateMesh(char* &mesh)
{
	assert(mesh != nullptr);
	DeleteMesh();

	unsigned int numIndices = *(int*)mesh;
	mesh += sizeof(int);
	unsigned int numVertices = *(int*)mesh;
	mesh += sizeof(int);

	float* vertices = (float*)mesh;
	mesh += sizeof(float) * 3 * numVertices;

	float* texCoords = (float*)mesh;
	mesh += sizeof(float) * 2 * numVertices;

	int* indices = (int*)mesh;
	mesh += sizeof(int) * numIndices;



	// vertex array objects (VAO)
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Divide Buffer for position and UVs
	glBufferData(GL_ARRAY_BUFFER, numVertices * (sizeof(float) * 3 + sizeof(float) * 2), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3 * numVertices, vertices);

	// Texture coords (UVs)
	// MapBufferRange because we only want UV data from UVW
	float * pbuffer = (float*)glMapBufferRange(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * numVertices, sizeof(GLfloat) * 2 * numVertices, GL_MAP_WRITE_BIT);
	memcpy(pbuffer, texCoords, sizeof(float) * 2 * numVertices);

	/*math::float2* texCoords = (math::float2*)glMapBufferRange(GL_ARRAY_BUFFER, sizeof(float) * 3 * src_mesh->mNumVertices,
		sizeof(float) * 2 * src_mesh->mNumVertices, GL_MAP_WRITE_BIT);

	for (unsigned j = 0; j < src_mesh->mNumVertices; ++j)
	{
		texCoords[j] = math::float2(src_mesh->mTextureCoords[0][j].x, src_mesh->mTextureCoords[0][j].y);
	}*/

	glUnmapBuffer(GL_ARRAY_BUFFER);

	// Indices (faces)
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, nullptr, GL_STATIC_DRAW);

	// Texture coords (UVs)
	// MapBufferRange because we only want UV data from UVW

	int * pbufferIndex = (int*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int)*numIndices, GL_MAP_WRITE_BIT);
	memcpy(pbufferIndex, indices, sizeof(int) * numIndices);
	/*
	unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0,
		sizeof(unsigned)*src_mesh->mNumFaces * 3, GL_MAP_WRITE_BIT);

	for (unsigned j = 0; j < src_mesh->mNumFaces; ++j)
	{
		assert(src_mesh->mFaces[j].mNumIndices == 3);

		*(indices++) = src_mesh->mFaces[j].mIndices[0];
		*(indices++) = src_mesh->mFaces[j].mIndices[1];
		*(indices++) = src_mesh->mFaces[j].mIndices[2];
	}
	*/
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
		(void*)(sizeof(float) * 3 * numVertices)       // array buffer offset
	);

	// Disable VAO
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	// Disable VBO and EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//gen_mesh->material = src_mesh->mMaterialIndex;
	num_vertices = numVertices;

	//Copying Vertices array
	this->vertices = new float[num_vertices * 3]; //It is checked below that at least has 1 face, so at least 3 vertices
	memcpy(this->vertices, vertices, sizeof(float)*num_vertices * 3);

	//Copying Face Normals
	/*if (src_mesh->HasNormals())
	{
		mesh->num_normals = src_mesh->mNumVertices;
		mesh->normals = new float[mesh->num_normals * 3];
		memcpy(mesh->normals, src_mesh->mNormals, sizeof(float)*mesh->num_normals * 3);
	}*/

	//Copying indices
	num_indices = numIndices;
	this->indices = (unsigned*)indices;
	/*
	this->indices = new unsigned[num_indices]; // assume each face is a triangle

	for (int j = 0; j < numFaces; ++j)
	{
		if (src_mesh->mFaces[j].mNumIndices != 3)
		{
			LOG("WARNING, geometry face with != 3 indices!");
			LOG("WARNING, face normals couldn't be loaded");
			mesh = nullptr;
			break;
		}
		else
		{
			memcpy(&mesh->indices[j * 3], src_mesh->mFaces[j].mIndices, 3 * sizeof(unsigned));
		}
	}*/

	boundingBox.SetNegativeInfinity();
	boundingBox.Enclose((math::float3*)vertices, num_vertices);
	App->resources->meshes.push_back(this);
}

// BUG: Draw meshes makes imgui flick
void ComponentMesh::RenderMesh(const math::float4x4& view, const math::float4x4& proj)
{
	Texture* texture = nullptr;

	//Draw meshes
	unsigned program = 0;

	if (my_go->material != nullptr)
		program = App->shader->programs[my_go->material->shader];
	else
		program = App->shader->programs[0];

	if (program < 1)
	{
		LOG("Program shader couldn't be found, it may not be loaded.");
		return;
	}

	glUseProgram(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, (const float*)&my_go->GetGlobalTransform()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (const float*)&view);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, (const float*)&proj);

	// Render Material
	if (my_go->material != nullptr && my_go->material->active)
	{
		my_go->material->RenderMaterial();
	}

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);

	// Disable VAO
	glBindVertexArray(0);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void ComponentMesh::DeleteMesh()
{
	if (vbo != 0)
	{
		glDeleteBuffers(1, &vbo);
	}

	if (ibo != 0)
	{
		glDeleteBuffers(1, &ibo);
	}

	if (vao != 0)
	{
		glDeleteBuffers(1, &vao);
	}
}

void ComponentMesh::Delete()
{
	my_go->mesh = nullptr;
	Component::Delete();
	App->resources->DeleteMesh(this);
}

void ComponentMesh::GenerateMesh(par_shapes_mesh_s* mesh)
{

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Positions

	/*for (unsigned i = 0; i< unsigned(mesh->npoints); ++i)
	{
		math::float3 point(mesh->points[i * 3], mesh->points[i * 3 + 1], mesh->points[i * 3 + 2]);
		// TODO: Set position
		//point = my_go->transform.TransformPos(point);
		for (unsigned j = 0; j<3; ++j)
		{
			min_v[j] = min(min_v[j], point[i]);
			max_v[j] = max(max_v[j], point[i]);
		}
	}*/

	unsigned offset_acc = sizeof(math::float3);
	unsigned normals_offset = 0;

	if (mesh->normals)
	{
		normals_offset = offset_acc;
		offset_acc += sizeof(math::float3);
	}

	glBufferData(GL_ARRAY_BUFFER, offset_acc*mesh->npoints, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float3)*mesh->npoints, mesh->points);

	// normals
	if (mesh->normals)
	{
		glBufferSubData(GL_ARRAY_BUFFER, normals_offset*mesh->npoints, sizeof(math::float3)*mesh->npoints, mesh->normals);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// indices

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(unsigned), indices , GL_STATIC_DRAW);

	unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0,
		sizeof(unsigned)*mesh->ntriangles * 3, GL_MAP_WRITE_BIT);

	for (unsigned i = 0; i< unsigned(mesh->ntriangles * 3); ++i)
	{
		*(indices++) = mesh->triangles[i];
	}

	// generate VAO
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	if (normals_offset != 0)
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(normals_offset*mesh->npoints));
	}

	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*vertices.reserve(mesh->npoints);
	for (unsigned int i = 0; i < mesh->npoints; i++)
	{
		vertices.push_back(float3((float *)&mesh->points[i]));
	}*/

	//materialIndex = 0;
	num_indices = mesh->ntriangles * 3;
	num_vertices = mesh->npoints;
}

JSON_value* ComponentMesh::Save(JSON_value* component) const
{
	JSON_value* mesh = Component::Save(component);

	mesh->AddUnsigned("VBO", vbo);
	mesh->AddUnsigned("IBO", ibo);
	mesh->AddUnsigned("VAO", vao);
	mesh->AddUnsigned("Number Vertices", num_vertices);
	mesh->AddUnsigned("Number Indices", num_indices);
	//TODO: Add vertices, indices and normals
	mesh->AddVec3("boundingBox min", boundingBox.minPoint);
	mesh->AddVec3("boundingBox max", boundingBox.maxPoint);

	component->addValue("", mesh);

	return mesh;
}

void ComponentMesh::Load(JSON_value* component)
{
	Component::Load(component);

	vbo = component->GetUnsigned("VBO");
	ibo = component->GetUnsigned("IBO");
	vao = component->GetUnsigned("VAO");
	num_vertices = component->GetUnsigned("Number Vertices");
	num_indices = component->GetUnsigned("Number Indices");
	//TODO: Add vertices, indices and normals
	boundingBox.minPoint = component->GetVec3("boundingBox min");
	boundingBox.maxPoint = component->GetVec3("boundingBox max");
}