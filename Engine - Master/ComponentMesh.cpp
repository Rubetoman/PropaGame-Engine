#include "ComponentMesh.h"

#include "Application.h"
#include "ModuleShader.h"
#include "ModuleResources.h"


#include "MeshImporter.h"

#include "ComponentTransform.h"
#include "ComponentMaterial.h"

#include "GameObject.h"

#include "par_shapes.h"

ComponentMesh::ComponentMesh(GameObject* go) : Component(go, component_type::Mesh)
{
}

ComponentMesh::ComponentMesh(const ComponentMesh& comp) : Component(comp)
{
	mesh = comp.mesh;
	currentMesh = comp.currentMesh;
	if(currentMesh.size() > 0)
		App->resources->meshes.push_back(this);
}

ComponentMesh::~ComponentMesh()
{
}

Component* ComponentMesh::Duplicate()
{
	return new ComponentMesh(*this);
}

void ComponentMesh::Delete()
{
	my_go->mesh_comp = nullptr;
	DeleteMesh();
	Component::Delete();

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
			if (ImGui::BeginCombo("##meshCombo", currentMesh.c_str()))
			{
				if (ImGui::Selectable("", (currentMesh.size() < 1)))
				{
					currentMesh = "";
					DeleteMesh();
				}
				for (std::vector<std::string>::iterator it = App->resources->file_meshes->begin(); it != App->resources->file_meshes->end(); ++it)
				{
					bool isSelected = (currentMesh == (*it));
					if (ImGui::Selectable((*it).c_str(), isSelected))
					{
						if (isSelected)
							ImGui::SetItemDefaultFocus();
						else
							LoadMesh((*it).c_str());
					}
				}
				ImGui::EndCombo();
			}

			ImGui::Text("Triangles Count: %d", mesh.num_indices / 3);
			ImGui::Text("Vertices Count: %d", mesh.num_vertices);
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

void ComponentMesh::LoadMesh(const char* name) 
{
	App->resources->DeleteMesh(this);
	if (mesh.vbo != 0)
		glDeleteBuffers(1, &mesh.vbo);

	if (mesh.ibo != 0)
		glDeleteBuffers(1, &mesh.ibo);

	MeshImporter::CleanUpMesh(&mesh);
	currentMesh.clear();
	MeshImporter::Load(&mesh, name);
	ComputeMesh();
	my_go->ComputeBBox();
	currentMesh = name;

	App->resources->meshes.push_back(this);
}

void ComponentMesh::ComputeMesh()
{
	glGenBuffers(1, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

	unsigned offset = sizeof(math::float3);

	if (mesh.normals != nullptr)
	{
		mesh.normalsOffset = offset;
		offset += sizeof(math::float3);
	}

	if (mesh.uvs != nullptr)
	{
		mesh.texturesOffset = offset;
		offset += sizeof(math::float2);
	}

	mesh.vertexSize = offset;

	glBufferData(GL_ARRAY_BUFFER, mesh.vertexSize * mesh.num_vertices, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * mesh.num_vertices, mesh.vertices);

	if (mesh.uvs != nullptr)
		glBufferSubData(GL_ARRAY_BUFFER, mesh.texturesOffset * mesh.num_vertices, sizeof(float) * 2 * mesh.num_vertices, mesh.uvs);

	if (mesh.normals != nullptr)
		glBufferSubData(GL_ARRAY_BUFFER, mesh.normalsOffset * mesh.num_vertices, sizeof(float) * 3 * mesh.num_vertices, mesh.normals);


	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &mesh.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.num_indices * sizeof(unsigned), mesh.indices, GL_STATIC_DRAW);

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &mesh.vao);

	glBindVertexArray(mesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	if (mesh.texturesOffset != 0)
	{
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.texturesOffset * mesh.num_vertices));
	}

	if (mesh.normalsOffset != 0)
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.normalsOffset * mesh.num_vertices));
	}

	glBindVertexArray(0);

	mesh.boundingBox.SetNegativeInfinity();
	mesh.boundingBox.Enclose((math::float3*)mesh.vertices, mesh.num_vertices);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ComponentMesh::ComputeMesh(par_shapes_mesh_s* parMesh) 
{
	assert(parMesh != nullptr);

	glGenBuffers(1, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

	unsigned offset = sizeof(math::float3);

	if (parMesh->normals) 
	{
		mesh.normalsOffset = offset;
		offset += sizeof(math::float3);
	}

	if (parMesh->tcoords)
	{
		mesh.texturesOffset = offset;
		offset += sizeof(math::float2);
	}

	mesh.vertexSize = offset;

	mesh.num_vertices = parMesh->npoints;
	mesh.num_indices = parMesh->ntriangles * 3;

	glBufferData(GL_ARRAY_BUFFER, mesh.vertexSize * mesh.num_vertices, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * mesh.num_vertices, parMesh->points);

	if (parMesh->tcoords)
		glBufferSubData(GL_ARRAY_BUFFER, mesh.texturesOffset * mesh.num_vertices, sizeof(float) * 2 * mesh.num_vertices, parMesh->tcoords);

	if (parMesh->normals)
		glBufferSubData(GL_ARRAY_BUFFER, mesh.normalsOffset * mesh.num_vertices, sizeof(float) * 3 * mesh.num_vertices, parMesh->normals);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &mesh.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * parMesh->ntriangles * 3, nullptr, GL_STATIC_DRAW);

	mesh.indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned) * parMesh->ntriangles * 3, GL_MAP_WRITE_BIT);

	for (unsigned i = 0; i< unsigned(parMesh->ntriangles * 3); ++i) 
	{
		*(mesh.indices++) = parMesh->triangles[i];
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &mesh.vao);

	glBindVertexArray(mesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	if (mesh.normalsOffset != 0)
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.normalsOffset * mesh.num_vertices));
	}

	if (mesh.texturesOffset != 0) 
	{
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.texturesOffset * mesh.num_vertices));
	}

	glBindVertexArray(0);

	mesh.boundingBox.SetNegativeInfinity();
	mesh.boundingBox.Enclose((math::float3*)parMesh->points, mesh.num_vertices);
	App->resources->meshes.push_back(this);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ComponentMesh::RenderMesh(const math::float4x4& view, const math::float4x4& proj)
{
	//Draw meshes
	unsigned program = 0;

	if (my_go->material_comp != nullptr)
		program = App->shader->programs[my_go->material_comp->shader];
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
	if (my_go->material_comp != nullptr && my_go->material_comp->active)
	{
		my_go->material_comp->RenderMaterial();
	}

	glBindVertexArray(mesh.vao);
	glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, nullptr);

	// Disable VAO
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void ComponentMesh::DeleteMesh()
{
	if (mesh.vbo != 0)
	{
		glDeleteBuffers(1, &mesh.vbo);
	}

	if (mesh.ibo != 0)
	{
		glDeleteBuffers(1, &mesh.ibo);
	}

	if (mesh.vao != 0)
	{
		glDeleteBuffers(1, &mesh.vao);
	}
	MeshImporter::CleanUpMesh(&mesh);
	currentMesh.clear();
	App->resources->DeleteMesh(this);
}

JSON_value* ComponentMesh::Save(JSON_value* component) const
{
	JSON_value* mesh = Component::Save(component);

	mesh->AddString("currentMesh", currentMesh.c_str());

	component->addValue("", mesh);

	return mesh;
}

void ComponentMesh::Load(JSON_value* component)
{
	Component::Load(component);

	currentMesh = component->GetString("currentMesh");

	LoadMesh(currentMesh.c_str());
}